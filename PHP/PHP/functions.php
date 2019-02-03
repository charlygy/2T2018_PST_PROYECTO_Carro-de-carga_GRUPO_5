<?php 
header( 'Content-Type: text/html;charset=utf-8' );



function getSQLResultSet($commando){
 
 
  $mysqli = new mysqli("localhost", "ivanb93", "12345", "pst_carro");
/* check connection */
if ($mysqli->connect_errno) {
    printf("Connect failed: %s\n", $mysqli->connect_error);
    exit();
}

if ( $mysqli->multi_query($commando)) {
	return $mysqli->store_result();
	
     
    
   
}



$mysqli->close();
}


?>
