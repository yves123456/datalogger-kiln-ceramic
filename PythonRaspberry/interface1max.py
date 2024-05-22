#!/usr/bin/env python
import tkinter as tk
import subprocess
import mysql.connector
from datetime import date
import time
#import sys

# Redirection de la sortie vers un fichier
#with open('output.txt', 'w') as f:
#    sys.stdout = f

# Connexion à la première base de données
mydb = mysql.connector.connect(
    host="localhost",
    user="",
    password="",
    database="datalogger"
)
mycursor = mydb.cursor()



# déclaration des variables
t = time.localtime()
current_time = time.strftime("%H:%M:%S", t)
today = date.today()
id_cuisson = 0
type_cuisson = "Gaz"
nom_prg_gaz = "debut_gaz_1tc.py"


# fonction début du programme
#def ajouter_nom(conn, conn2):
def ajouter_nom(conn):
    global id_cuisson
    nom = nom_entry.get()
    description = description_entry.get()
    operateur = operateur_entry.get()
    
    
    if nom:
        try:
            cursor = conn.cursor()
            cursor.execute("INSERT INTO cuisson (date, nom, operateur, description, type, date_deb, heure_deb) VALUES (%s, %s, %s, %s, %s, %s, %s)", (today, nom, operateur, description, type_cuisson, today, current_time,))
            conn.commit()
          

            cursor.execute("SELECT id_cuisson FROM cuisson ORDER BY id_cuisson DESC LIMIT 1")
            result = cursor.fetchone()[0]
            id_cuisson = str(result)
            #print(id_cuisson + " haut")

            global process
            try:
                nom_prg = nom_prg_gaz
                process = subprocess.Popen(["python", nom_prg], stdin=subprocess.PIPE)

                # Envoi de la variable au processus enfant
                process.stdin.write(id_cuisson.encode())
                process.stdin.close()

                ajouter_button.config(state=tk.DISABLED)
                stop_button.config(state=tk.NORMAL)

            except Exception as e:
                print("Erreur lors du démarrage du programme :", e)
            status_label.config(text="Debut enregistrement", fg="green")

        except mysql.connector.Error as err:
            status_label.config(text=f"Erreur: {err}", fg="red")
    else:
        status_label.config(text="Veuillez saisir un nom", fg="red")

# Fonction pour exécuter la requête SQL et afficher les résultats dans l'interface Tkinter
def afficher_resultats(conn):
    try:
        cursor = conn.cursor()
        cursor.execute("SELECT Heure, templu, templua FROM `data1max` WHERE id_cuisson = %s ORDER BY id DESC LIMIT 1", (id_cuisson,))
        
        rows = cursor.fetchall()

        for widget in labels:
            widget.destroy()

        for i, row in enumerate(rows):
            heure, templu, templua = row
            label = tk.Label(frame, text=f"Heure: {heure}, TempLu: {templu}, TempLua: {templua}")
            label.grid(row=6, column=0, sticky="w")
            labels.append(label)

    except mysql.connector.Error as err:
        #print("Erreur lors de l'accès à la base de données :", err)
        status_label.config(text="Erreur lors de l'accès à la base de données", fg="red")
    root.after(60000, afficher_resultats, conn)

#def stop_program(conn,conn2):
def stop_program(conn):
    global id_cuisson
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    today = date.today()

    try:
        cursor = conn.cursor()
        cursor.execute("UPDATE cuisson SET date_fin = %s, heure_fin = %s WHERE id_cuisson = %s", (today, current_time, id_cuisson))
        conn.commit()
        
        

        if process:
            process.terminate()
            ajouter_button.config(state=tk.NORMAL)
            stop_button.config(state=tk.DISABLED)
            status_label.config(text="Fin enregistrement", fg="red")

    except Exception as e:
        #print("Erreur lors de l'arrêt du programme :", e)
        status_label.config(text="Erreur lors de l'arrêt du programme", fg="red")

root = tk.Tk()
root.title("Lancement du programme GAZ")
root.geometry("700x400")

frame = tk.Frame(root)
frame.pack(padx=20, pady=20)

labels = []  # Liste pour stocker les labels créés

nom_label = tk.Label(frame, text="Nom cuisson:")
nom_label.grid(row=0, column=0, sticky="w")

operateur_label = tk.Label(frame, text="Operateur:")
operateur_label.grid(row=1, column=0, sticky="w")

description_label = tk.Label(frame, text="Description:")
description_label.grid(row=2, column=0, sticky="w")

nom_entry = tk.Entry(frame)
nom_entry.grid(row=0, column=1, padx=10)

operateur_entry = tk.Entry(frame)
operateur_entry.grid(row=1, column=1, padx=10)

description_entry = tk.Entry(frame)
description_entry.grid(row=2, column=1, padx=10)

ajouter_button = tk.Button(frame, text="Début", command=lambda: ajouter_nom(mydb))
ajouter_button.grid(row=4, columnspan=2, pady=10)

stop_button = tk.Button(frame, text="Fin", command=lambda: stop_program(mydb), state=tk.DISABLED)

stop_button.grid(row=5, columnspan=2, pady=10)

status_label = tk.Label(frame, text="", fg="black")
status_label.grid(row=9, columnspan=2, pady=10)

# Appeler la fonction pour afficher les résultats
afficher_resultats(mydb)

root.mainloop()
