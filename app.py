from flask import Flask, jsonify ,render_template
import mysql.connector
import pymysql.cursors


app = Flask(__name__)

# ---------Configuration de la connexion à MariaDB --------------

db_config = {
    "host": "192.168.9.150",
    "user": "admin",
    "password": "admin",
    "database": "mat_eolien_systeme"
}

def get_db_connection():
    return mysql.connector.connect(**db_config)


@app.route("/", methods=["GET"])
def index():
   return render_template('index.html')

# Route API pour récupérer les dernières mesures--------------------
@app.route("/m1", methods=["GET"])
def get_mesures1():
    connection = pymysql.connect(**db_config)
    cursor = connection.cursor()


    cursor.execute("SELECT vitesse_du_vent, unite, direction_du_vent , date_heure  FROM mesures ")
    mesures = cursor.fetchall()

    connection.close()
    return render_template('/module1.html', mesures=mesures)

@app.route("/m2", methods=["GET"])
def get_mesures2():
    connection = pymysql.connect(**db_config)
    cursor = connection.cursor()


    cursor.execute("SELECT vitesse_du_vent, unite, direction_du_vent , date_heure  FROM mesures ")
    mesures = cursor.fetchall()

    connection.close()
    return render_template('/module2.html', mesures=mesures)

@app.route("/m3", methods=["GET"])
def get_mesures3():
    connection = pymysql.connect(**db_config)
    cursor = connection.cursor()


    cursor.execute("SELECT vitesse_du_vent, unite, direction_du_vent , date_heure  FROM mesures ")
    mesures = cursor.fetchall()

    connection.close()
    return render_template('/module3.html', mesures=mesures)

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=5000)

