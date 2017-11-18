import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.positiveList = []
        self.negativeList = []

        with open(positives) as lines:
            for line in lines:
                line = line.rstrip('\n')
                if(not (line.startswith(';')) and line):
                    self.positiveList.append(line)

        with open(negatives) as lines:
            for line in lines:
                line = line.rstrip('\n')
                if(not (line.startswith(';')) and line):
                    self.negativeList.append(line)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        for word in tokens:
            word = word.lower()
            if(word in self.positiveList):
                score += 1
            elif(word in self.negativeList):
                score += -1
        return score
