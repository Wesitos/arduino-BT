<?php
require 'connect.php';

// Set the JSON header
header("Content-type: text/json");
//Lista de tablas
$l_tablas= array(
		 'temp0',
		 'temp1',
		 'temp2',
		 'temp3',
		 'temp4',
		 );
//Lee los parametros get (table y date)
$table = filter_input(INPUT_GET, 'table', FILTER_SANITIZE_SPECIAL_CHARS);
$min_date =(int)filter_input(INPUT_GET, 'date', FILTER_SANITIZE_NUMBER_INT)/1000;

//Prepara la peticion
$sql = 'SELECT * FROM '. $l_tablas[(int)$table].' Where date > ?';
$stmt = $conn->prepare($sql);

//Agrega los parametros y ejecuta la petición a la base de datos
$stmt->bind_param('s', date("Y-m-d G:i:s", $min_date) );
$stmt->execute();


$stmt->bind_result($date,$valor);
// Create a PHP array and echo it as JSON
$arr = array();

while ($row = $stmt->fetch() )
  {
    $fecha = new DateTime($date);
    $arr[]=array($fecha->getTimestamp()*1000,$valor);
  }
$stmt->close();
echo json_encode($arr);

?>