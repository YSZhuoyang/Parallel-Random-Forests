#!flask/bin/python
"""Wrap and expose API interfaces for sentiment analysis."""

from flask import Flask
#from RandomForestsSenAnalysis import invoke_rf_test, invoke_rf_train

APP = Flask(__name__)

@APP.route('/')

def index():
    """Random forest test api."""
    #invoke_rf_test()
    return 'rf test finished'

@APP.route('/rf_train')

def rf_train():
    """Random forest training api."""
    #invoke_rf_train(5)
    return 'rf training finished'

if __name__ == '__main__':
    APP.run(debug=True)

