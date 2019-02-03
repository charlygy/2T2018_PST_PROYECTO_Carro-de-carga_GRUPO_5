<?php
include('functions.php');
$id_usuario=$_GET["id_usuario"];


if($resultset=getSQLResultSet("SELECT id_usuario,contrasena FROM `usuario` WHERE id_usuario='$id_usuario'")){
	while ($row = $resultset->fetch_array(MYSQLI_NUM)){
		echo json_encode($row);
	}
}

?>


