<?php
require_once __DIR__."/includes/variables.php";
require_once __DIR__."/songHandler/SongHandler.php";

session_start();
if (!isset($_SESSION['user'])) {
	header("Location: restricted.php?location=".$_SERVER['REQUEST_URI']);
	die();
}

if (!isset($_GET['id'])) {
	header("Location: ".$BASE);
	die();
}

$songHandler = new SongHandler();

try {
	$product = $songHandler->getProductMinimum($_GET['id']);
} catch (Exception $ex) {
	echo "Something went wrong. Please report the following to us: ". $ex->getMessage();
}

if ($product == 0) {
	header("Location: ".$BASE);
	die();
}

else {
	$product = $product[0];
	require_once __DIR__."/template/download_template.php";
}

