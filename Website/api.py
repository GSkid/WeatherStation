from flask import Flask, render_template, request, jsonify
import csv

def create_app(): 
    app = Flask(__name__)

    # Error routing
    @app.errorhandler(404)
    def page_not_found(error):
        return 'This route does not exist {}'.format(request.url), 404
    
    # Home page uses index.html
    @app.route("/")
    def home():
        return render_template("index.html")


    # About me page uses about_me.html
    @app.route("/about_me")
    def about_me():
        return render_template("about_me.html")

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
    return app
    


#### MAIN ####
# if __name__ == "__main__":
#    app.run(debug=true)
