import mysql.connector
# Import de la librairie serial
import serial
import re
from datetime import date
import time
import sys
#import smtplib
#import ssl

#import os
from sendgrid import SendGridAPIClient
from sendgrid.helpers.mail import Mail

# Remplacez cette valeur par votre clé API SendGrid
SENDGRID_API_KEY = ''

# Paramètres de l'e-mail
sender_email = ''
receiver_email = ''
subject = 'Sujet de l\'e-mail'
message_body = 'Contenu de l\'e-mail.'
# Configuration de l'e-mail
temp_max = 0
res = [0] * 15
test_mail = ""
debut = 1
# Lecture de la variable depuis l'entrée standard
id_fourne = sys.stdin.read()

# Ouverture du port serie avec :
# '/dev/ttyXXXX' : definition du port d ecoute (remplacer 'X' par le bon nom)
# 9600 : vitesse de communication
serialArduino = serial.Serial('/dev/ttyUSB0', 9600)
#serialArduino.close()

mydb = mysql.connector.connect(
    host="localhost",
    user="",
    password="",
    database="datalogger"
)
mycursor = mydb.cursor()

port = 3307


if debut == 1 :
   debut = 0
   t = time.localtime()
   current_time = time.strftime("%H:%M:%S", t)
   today = date.today()
   sql = "insert into data4maxtk(Date,Heure,T1,T2,T3,T4,E1,E2,E3,E4,id_cuisson) values(%s, %s, %s, %s,%s, %s, %s, %s, %s,%s, %s)"
   val = (today, current_time, "0", "0", "0", "0", "0", "0", "0", "0", id_fourne)
   mycursor.execute(sql, val)
   mydb.commit()

# Ecriture de chaque message recu
while True:
    #print(serialArduino.readline())
    try:
        temp = serialArduino.readline()
        print("Ligne lue avec succès :", temp)
    except serial.SerialException as e:
        print("Erreur lors de la lecture depuis la connexion série :", e)
        temp = "b'0 0 0 0 0 0 0 0\r\n'"
        #serialArduino.close()
        #serialArduino = serial.Serial('/dev/ttyUSB0', 9600)
    #finally:
    #   # Fermer la connexion série
    #   serialArduino.close()

    temp = str(temp)
    #print(temp + " avant")
    #print (len(temp))
    if len(temp) < 21:
        temp = "b'0 0 0 0 0 0 0 0\r\n'"
    if "Thermocouple" in temp :
        temp = "b'0 0 0 0 0 0 0 0\r\n'"    
    #else:
    #    print(temp + " après")
    #print(temp + " après 2")

    temp = re.sub('[^a-zA-Z-0-9-,-:-.]', ' ', temp)
    #print(temp)
    temp = "'" + temp + "'"

    heure = temp[2:11]
    #print(heure)

    res = temp.split()
    #print(res)
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    today = date.today()

    #print(current_time)
    #print(today)

    print(res[1])
    print(res[2])
    print(res[3])
    print(res[4])
    id_fourne = int(id_fourne)

   


    if res[1] == "0" :
        print ("Erreur pas d'enregistrement")
        sql = ""
        val = ""
    else :    
        sql = "insert into data4maxtk(Date,Heure,T1,T2,T3,T4,E1,E2,E3,E4,id_cuisson) values(%s, %s, %s, %s,%s, %s, %s, %s, %s,%s, %s)"
        val = (today, current_time, res[1], res[2], res[3], res[4], res[5], res[6], res[7], res[8], id_fourne)
        temp_max = res[3]
        temp_max = float(temp_max)
    #print(sql)

        mycursor.execute(sql, val)
        mydb.commit()

 
    # Appel de la fonction pour envoyer l'e-mail
    # Configuration de l'API SendGrid

    message_body = "Température" + str(temp_max) + "fin"
    subject = 'Datalogger'

    message = Mail(
        from_email=sender_email,
        to_emails=receiver_email,
        subject=subject,
        plain_text_content=message_body
    )

    if temp_max > 1220 and test_mail == 0:
        try:
            sg = SendGridAPIClient(SENDGRID_API_KEY)
            response = sg.send(message)
            print("E-mail envoyé avec succès!")
            test_mail = 1
        except Exception as e:
            print("Erreur lors de l'envoi de l'e-mail:", e)

    #else:
    #    print("ok")

    if temp_max < 1220:
        test_mail = 0
