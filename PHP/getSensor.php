<?php

$servername = "185.179.190.243";
$username = "simpleuser";
$password = "Q1a4X8g2";
$dbname = "alexn_sensors";

// Getting data from request:
$temp = $_GET['temp'];
$hum = $_GET['hum'];
$date = $_GET['date'];

$sqlRequest = "INSERT INTO alexn_sensors.room_temp_sensor(humidity,temperature,date) VALUES(".$hum . ",".$temp.",'".$date."')";

// Create connection
$conn = new mysqli($servername, $username, $password);
// Check if all right:

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
} 
echo "<br>";
echo "Connected successfully ";
echo "<br>";

$result = $conn->query($sqlRequest);
echo "<br>";
echo "==== INSERTING DATA === ";
echo "<br>";
echo $sqlRequest;
echo "<br>";
$conn->close();

?>
