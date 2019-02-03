<?php
include('functions.php');
$id_usuario=$_GET["id_usuario"];


if($resultset=getSQLResultSet("SELECT nombre,peso FROM `item` WHERE id_usuario='$id_usuario'")){
	while ($row = $resultset->fetch_array(MYSQLI_NUM)){
		echo json_encode($row);
	}
}

?>


