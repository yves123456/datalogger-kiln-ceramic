import mysql.connector
import serial
import re
from datetime import date
import time
import sys
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
res = [0] * 8
test_mail = ""
debut = 1

# Lecture de la variable depuis l'entrée standard
id_fourne = sys.stdin.read()

# Ouverture du port serie avec :
# '/dev/ttyXXXX' : definition du port d ecoute (remplacer 'X' par le bon nom)
# 9600 : vitesse de communication
serialArduino = serial.Serial('/dev/ttyUSB0', 9600)

mydb = mysql.connector.connect(
    host="localhost",
    user="",
    password="",
    database="datalogger"
)
mycursor = mydb.cursor()

if debut == 1 :
   debut = 0
   t = time.localtime()
   current_time = time.strftime("%H:%M:%S", t)
   today = date.today()
   sql = "insert into data1max(Date,Heure,templu,templua,templuavant,delta,pente,tchs1,id_cuisson) values(%s, %s, %s, %s,%s, %s, %s, %s, %s)"
   val = (today, current_time, "0", "0", "0", "0", "0", "0", id_fourne)
   mycursor.execute(sql, val)
   mydb.commit()




# Ecriture de chaque message recu
while True:
    try:
        temp = serialArduino.readline()
        print("Ligne lue avec succès :", temp)
    except serial.SerialException as e:
        print("Erreur lors de la lecture depuis la connexion série :", e)
        temp = "b'0 0 0 0 0 0\r\n'"

    temp = str(temp)
    #print(temp + " avant")
    if len(temp) < 10  :
        temp = "b'0 0 0 0 0 0\r\n"
    #else:
    #    print(temp + " après")
    #print(temp + " après 2")

    temp = re.sub('[^a-zA-Z-0-9-,-:-.]', ' ', temp)
    temp = "'" + temp + "'"

    heure = temp[2:11]

    res = temp.split()
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    today = date.today()

    print(res[1])
    print(res[2])
    print(res[3])


    id_fourne = int(id_fourne)

    sql = "insert into data1max(Date,Heure,templu,templua,templuavant,delta,pente,tchs1,id_cuisson) values(%s, %s,%s, %s, %s, %s, %s, %s, %s)"
    val = (today, current_time, res[1], res[2], res[3], res[4], res[5], res[6], id_fourne)

    temp_max = res[3]
    temp_max = float(temp_max)

    mycursor.execute(sql, val)
    mydb.commit()

    #mycursor_2.execute(sql, val)
    #mydb_2.commit()

    message_body = "Température" + str(temp_max) + "fin"
    subject = 'Datalogger'

    message = Mail(
        from_email=sender_email,
        to_emails=receiver_email,
        subject=subject,
        plain_text_content=message_body
    )

    if temp_max > 1300 and test_mail == 0:
        try:
            sg = SendGridAPIClient(SENDGRID_API_KEY)
            response = sg.send(message)
            print("E-mail envoyé avec succès!")
            test_mail = 1
        except Exception as e:
            print("Erreur lors de l'envoi de l'e-mail:", e)
    #else:
    #    print("ok")

    if temp_max < 1300:
        test_mail = 0
