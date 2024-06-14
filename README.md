Full article on the production available here : http://yves-lg.com/index.php/outillage/pottery-kiln-thermometer 
Contact yves.malibu@hotmail.com

    Arduino Program:
        The Arduino program is designed to read temperature data from one or four thermocouples.
        It communicates this data to a Raspberry Pi using serial communication.
        The program continuously reads temperature values and sends them to the Raspberry Pi at regular intervals.
        The data is formatted and transmitted via the serial port for further processing.

    Python Program:
        The Python program running on the Raspberry Pi receives temperature data from the Arduino via serial communication.
        It establishes a connection to a MySQL database to store the received data.
        The program parses the incoming data, extracts temperature values, and timestamps them.
        It then inserts the temperature readings into the appropriate table in the MySQL database for storage.
        Additionally, the program includes functionality to send email alerts based on predefined temperature thresholds.

    PHP Program:
        The PHP program serves as a web-based interface for visualizing the temperature data collected by the Arduino and stored in the MySQL database.
        It retrieves the latest temperature readings and firing session details from the database.
        The program dynamically generates progress bars to represent temperature levels and displays them on the web page.
        Users can interact with the interface to filter data by firing session ID and view temperature readings for specific sessions.
        Additionally, the program generates a temperature curve using JavaScript for dynamic visualization.

Each of these programs plays a crucial role in the end-to-end process of collecting, storing, and visualizing temperature data from the pottery kiln, providing a comprehensive solution for monitoring and analyzing the firing process.
