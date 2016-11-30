<?php
require_once __DIR__."/includes/variables.php";
require_once __DIR__."/songHandler/SongHandler.php";

if (!isset($_GET['id'])) {
	header("Location: ".$BASE);
}

$songHandler = new SongHandler();

try {
	$product = $songHandler->getProduct($_GET['id']);
} catch (Exception $ex) {
	echo "Something went wrong. Please report the following to us: ". $ex->getMessage();
}

if ($product == 0) {
	header("Location: ".$BASE);
}

else {
	$product = $product[0];
	require_once __DIR__."/template/product_template.php";
}

