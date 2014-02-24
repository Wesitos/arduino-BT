<?php

$DBServer = 'localhost';
$DBUser = 'python';
$DBPass = 'python';
$DBName = 'python';

$conn = new mysqli($DBServer, $DBUser, $DBPass, $DBName);


// Check connection

if ($conn->connect_error)
    trigger_error('Database connection failed: '.$conn->connect_error, E_USER_ERROR);

		   