<?php
require_once __DIR__."/includes/variables.php";

session_start();
if (isset($_SESSION['user']) && !isset($_GET['location'])) {
	header("Location: ".$BASE);
	die();
}

else if (isset($_SESSION['user'])) {
	header("Location: ".$_GET['location']);
	die();
}

else {
	require_once __DIR__."/template/restricted_template.php";
}