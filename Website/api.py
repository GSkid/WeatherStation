from flask import Flask, render_template, request, jsonify
import csv

app = Flask(__name__)

# Home page uses index.html
@app.route("/")
def home():
    return render_template("index.html")

# This method allows the user to get the json data from the web app
@app.route("/api_get")
def api_get():
    # Uses the csv module to put the data in the csv into a dictonary
    csvLoader = csv.DictReader(open('WeatherData.csv', 'r'))
    jsonDict = dict()
    i = 0
    # Now put the data into the actual dict
    for line in csvLoader:
        # Only want the first line of the dict
        if (i >= 1):
            break
        jsonDict = line
        i += 1
    # At the end, return the jsonified dictionary
    return jsonify(jsonDict)
    
if __name__ == "__main__":
    app.run(debug=True)
