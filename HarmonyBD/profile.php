<?php
require_once __DIR__."/includes/variables.php";
require_once __DIR__."/songHandler/SongHandler.php";

session_start();
if (!isset($_SESSION['user'])) {
	header("Location: restricted.php?location=".$_SERVER['REQUEST_URI']);
	die();
}

else {
	require_once __DIR__."/template/profile_template.php";
}

