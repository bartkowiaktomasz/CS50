# API KEY: XC8831435YLHUM5A
# export API_KEY=XC8831435YLHUM5A
import os
import time

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = db.execute("SELECT cash FROM users WHERE id=:user_id",
                   user_id=session["user_id"])[0]["cash"]

    stocks = db.execute("SELECT * FROM portfolio WHERE id=:user_id",
                   user_id=session["user_id"])

    for stock in stocks:
        result = lookup(stock["share"])
        if result is not None:
            price = result["price"]
            total = price*stock["amount"]
            db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:user_id AND share=:share",
                        price=price, total=total, user_id=session["user_id"], share=stock["share"])

    # Update stocks
    stocks = db.execute("SELECT * FROM portfolio WHERE id=:user_id",
                   user_id=session["user_id"])

    transactions = db.execute("SELECT transaction_id FROM portfolio WHERE id=:user_id", user_id=session["user_id"])
    portfolio_value = 0
    if transactions:
        portfolio_value = db.execute("SELECT SUM(total) AS Total FROM portfolio WHERE id = :user_id",
                        user_id=session["user_id"])[0]["Total"]

    total_value = cash + portfolio_value
    return render_template("index.html", stocks=stocks, cash=cash, total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        result = lookup(request.form.get("symbol"))

        if not result:
            return apology("Provide a symbol!", 400)

        try:
            amount = int(request.form.get("shares"))
        except ValueError:
            return apology("Not an integer!", 400)

        if not amount > 0:
            return apology("Not a positive integer!", 400)

        cash = db.execute("SELECT cash FROM users WHERE id=:user_id",
                           user_id=session["user_id"])[0]["cash"]

        price_per_shape = result["price"]
        total_price = amount * price_per_shape

        # Not enough money
        if total_price > cash:
            return apology("No money mate!")

        stock = db.execute("SELECT * FROM portfolio WHERE id=:user_id AND share=:share",
                            user_id=session["user_id"], share=result["symbol"])

        # If you don't already possess this stock, insert new entry in the table
        if not stock:
            transaction_id = db.execute("INSERT INTO portfolio (id, share, amount, price, total) VALUES(:user_id, :share, :amount, :price, :total)",
                    user_id=session["user_id"],
                    share=result["symbol"],
                    amount=amount,
                    price=result["price"],
                    total=total_price)

            db.execute("UPDATE users SET cash = cash - :price",
                        price=total_price)

            item = db.execute("SELECT * FROM portfolio WHERE transaction_id=:transaction_id",
                               transaction_id=transaction_id)
            # Add entry to hitory
            db.execute("INSERT INTO history (user_id, transaction_type, share, price, amount) \
                VALUES(:user_id, :transaction_type, :share, :price, :amount)",
                user_id=session["user_id"],
                transaction_type="Buy",
                share=item[0]["share"],
                price=item[0]["price"],
                amount=amount)

        # If you already possess this stock just update the amount
        else:
            already_amount = stock[0]["amount"]
            db.execute("UPDATE portfolio SET amount=:amount WHERE id=:user_id AND share=:share",
                        amount=amount+already_amount,
                        user_id=session["user_id"],
                        share=result["symbol"])

            db.execute("INSERT INTO history (user_id, transaction_type, share, price, amount) \
                    VALUES(:user_id, :transaction_type, :share, :price, :amount)",
                    user_id=session["user_id"],
                    transaction_type="Buy",
                    share=stock[0]["share"],
                    price=stock[0]["price"],
                    amount=amount)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM history WHERE user_id=:user_id",
                               user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")
        # return render_template("login.html", code=200)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Provide a symbol!")

        result = lookup(request.form.get("symbol"))
        if not result:
            return apology("No such symbol!")
        else:
            return render_template("quoted.html", symbol=result["symbol"], price=usd(result["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("Missing username!", 400)

        elif not request.form.get("password"):
            return apology("Missing password!", 400)

        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("Password and password confirmation do not match!", 400)

        hash = generate_password_hash(request.form.get("password"))
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                            username=request.form.get("username"),
                            hash=hash)

        if not result:
            return apology("User already exists in the database!", 400)
        else:
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]

        return redirect("/", code=200)
    else:
        return render_template("register.html", code=200)



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Provide a symbol!")

        result = lookup(request.form.get("symbol"))
        amount = int(request.form.get("shares"))

        if not result:
            return apology("No such symbol!")

        else:
            stock = db.execute("SELECT * FROM portfolio WHERE id=:user_id AND share=:share",
                                    user_id=session["user_id"], share=result["symbol"])
            user_owns = int(stock[0]["amount"])
            if user_owns < amount:
                return apology("Not enough shares to sell")

            else:
                cash = db.execute("SELECT cash FROM users WHERE id=:user_id",
                                    user_id=session["user_id"])[0]["cash"]
                db.execute("UPDATE users SET cash=:cash WHERE id=:user_id",
                            cash=cash+amount*result["price"], user_id=session["user_id"])

                # If user owns more than sells, update the entry
                if user_owns > amount:
                    item = db.execute("UPDATE portfolio SET amount=:amount WHERE id=:user_id AND share=:share",
                            amount=user_owns-amount,
                            user_id=session["user_id"],
                            share=result["symbol"])

                # Else delete the row
                else:
                    db.execute("DELETE FROM portfolio WHERE id=:user_id AND share=:share",
                                user_id=session["user_id"],
                                share=result["symbol"])

                # Add entry to hitory
                db.execute("INSERT INTO history (user_id, transaction_type, share, price, amount) \
                        VALUES(:user_id, :transaction_type, :share, :price, :amount)",
                        user_id=session["user_id"],
                        transaction_type="Sell",
                        share=stock[0]["share"],
                        price=stock[0]["price"],
                        amount=amount)

                return redirect("/")
    else:
        stocks = db.execute("SELECT * FROM portfolio WHERE id=:user_id",
                            user_id=session["user_id"])
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
