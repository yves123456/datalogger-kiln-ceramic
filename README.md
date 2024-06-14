Full article on the production available here : http://yves-lg.com/index.php/outillage/pottery-kiln-thermometer 
Contact yves.malibu@hotmail.com

Optimize Your Pottery Firing: A Complete Guide to Making and Using a Datalogger with a Thermocouple

In the field of pottery, precisely controlling the temperature during firing is crucial to achieve reproducible results and explore new glaze ranges. To achieve this level of control, using a datalogger that displays temperatures and records data for your pottery kiln is very useful. This article explains the making of this tool, from design to practical use, and the benefits it offers to potters.
The datalogger box for pottery kiln has several objectives:

    Track a firing and see its curve in real-time: Allows instant visualization of temperature variations and adjustments accordingly.
    Analyze the results after firing: By comparing temperature curves with final results, it is possible to optimize firing processes.
    Note events during firing: Record notes for future adjustments or identify specific issues.
    Have a history of firing curves: For future reference and improving consistency of results.
    Monitor firing in real-time: Ensure everything is going well and intervene in case of problems.

Necessary Components

    Arduino board: Base platform for connecting other components.
    MAX31856 module: Interface for thermocouples.
    Type S or K thermocouples: Temperature sensors.
    LED or LCD display: To visualize data in real-time.
    SD card (optional): To record data.
    Waterproof case: To protect electronic components.
    Additional Arduino modules (optional): To add features like barometric measurements.
    Raspberry Pi: For data recording, web server, database server, and data processing with Python and PHP.


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
