Program 1: Data Collection and Email Notification (Python with MySQL)

This Python script is designed to collect temperature data from an Arduino connected via serial communication. It then stores this data in a MySQL database and sends an email notification if a certain temperature threshold is exceeded.

    Libraries Used:
        mysql.connector: Used for connecting to and interacting with the MySQL database.
        serial: Used for serial communication with the Arduino.
        re: Used for regular expressions.
        datetime: Used for handling date and time.
        time: Used for handling time-related operations.
        sys: Used for accessing system-specific parameters and functions.
        sendgrid: Used for sending email notifications via the SendGrid API.

    Functionality:
        Connects to the Arduino via serial communication.
        Reads temperature data from the Arduino and stores it in a MySQL database.
        Checks if the temperature exceeds a predefined threshold and sends an email notification if it does.
        Handles exceptions and errors gracefully.

Program 2: Tkinter GUI for Firing Session Management (Python with MySQL)

This Python script provides a graphical user interface (GUI) for managing firing sessions in a pottery kiln. It allows users to start and stop recording sessions, input session details, and view real-time temperature data from the kiln.

    Libraries Used:
        tkinter: Used for creating the GUI.
        subprocess: Used for starting and communicating with the Arduino data collection script.
        mysql.connector: Used for connecting to and interacting with the MySQL database.
        datetime: Used for handling date and time.
        time: Used for handling time-related operations.

    Functionality:
        Allows users to input details for a firing session, such as name, operator, and description.
        Starts and stops the Arduino data collection script based on user input.
        Displays real-time temperature data from the kiln during a firing session.
        Handles errors and exceptions during database operations and script execution.

These Python scripts together form a system for monitoring and managing pottery kiln firing sessions, including data collection, storage, and user interface for interaction.