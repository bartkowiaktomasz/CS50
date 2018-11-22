import os
import re
from flask import Flask, jsonify, render_template, request

from cs50 import SQL
from helpers import lookup

# Configure application
app = Flask(__name__)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    return render_template("index.html")


@app.route("/articles")
def articles():
    """Look up articles for geo"""
    if request.method == "GET":
        geo = request.args['geo']
        articles = lookup(geo)
        return jsonify(articles)


@app.route("/search")
def search():
    """Search for places that match query"""
    if request.method == "GET":
        param = request.args.get("q")
        print(param)
        # TODO improve the algorithm (now only US supported)
        # Post code
        if param.isdigit():
            param += "%"
            places = db.execute("SELECT * FROM places WHERE postal_code LIKE :q",
                                q=param)

        else:
            params = param.split(' ')
            params = [elem.replace(',', '') for elem in params]

            if len(params) == 1:
                place = params[0] + '%'
                places = db.execute("SELECT * FROM places WHERE place_name LIKE :q",
                     q=place)

            else:
                place = params[0]
                admin_name = params[1] + '%'
                places = db.execute("SELECT * FROM places WHERE place_name=:p AND (admin_name1 LIKE :a OR admin_name2 LIKE :a)",
                                    p=place, a=admin_name)

                if len(places) == 0:
                    places = db.execute("SELECT * FROM places WHERE place_name=:p",
                                        p=params[0])

                # Situation where place consists of two words
                if len(places) == 0 and len(params) == 2:
                    place = params[0] + ' ' + params[1]
                    places = db.execute("SELECT * FROM places WHERE place_name=:p",
                                        p=place)

                if len(places) == 0 and len(params) == 3:
                    place = params[0] + ' ' + params[1]
                    admin_name = params[2] + '%'
                    places = db.execute("SELECT * FROM places WHERE place_name=:p AND (admin_name1 LIKE :a OR admin_name2 LIKE :a)",
                                        p=place, a=admin_name)

        return jsonify(places)


@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # Crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # Output places as JSON
    return jsonify(rows)
