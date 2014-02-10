<?php

require 'connect.php';
// Set the JSON header
header("Content-type: text/json");
// The x value is the current JavaScript time, which is the Unix time multiplied
$l_tablas= array('temp0',
		 'bluetooth',
		 );
//Lee parametros get
$table = filter_input(INPUT_GET, 'table', FILTER_SANITIZE_SPECIAL_CHARS);
$min_date = date("Y-m-d G:i:s", (int)filter_input(INPUT_GET, 'date', FILTER_SANITIZE_NUMBER_INT) );
//Prepara la peticion
$sql = 'SELECT * FROM '. $l_tablas[(int)$table].' Where date > ?';
$stmt = $conn->prepare($sql);


//Agrega los parametros
$stmt->bind_param('s',$min_date);
$stmt->execute();
$stmt->bind_result($date,$valor);
// Create a PHP array and echo it as JSON
$arr = array();

while ($row = $stmt->fetch() )
  {
    $fecha = new DateTime($date);
    $arr[]=array($fecha->getTimestamp(),$valor);
  }
$stmt->close();
echo json_encode($arr);
