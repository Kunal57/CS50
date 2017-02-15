import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.positives = []

        with open(positives) as lines:
            for line in lines:
                if line[0].isalpha():
                    self.positives.append(line.strip("\n"))

        self.negatives = []

        with open(negatives) as lines:
            for line in lines:
                if line[0].isalpha():
                    self.negatives.append(line.strip("\n"))


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        tokens = nltk.word_tokenize(text.lower())
        for token in tokens:
            if token in self.positives:
                score += 1
            elif token in self.negatives:
                score -= 1
            else:
                continue

        return score
