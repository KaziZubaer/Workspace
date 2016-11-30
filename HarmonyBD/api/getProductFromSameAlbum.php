<?
require_once __DIR__."/../includes/variables.php";
require_once __DIR__."/../songHandler/SongHandler.php";

if (!isset($_GET['id'])) {
	header("Location: ".$BASE);
}

$result = array(
	'success' => true, 
	'message' => '',
	'products' => []
);

$songHandler = new SongHandler();

try {
	$result['products'] = $songHandler->getProductFromSameAlbum($_GET['id']);
} catch (Exception $ex) {
	$result['success'] = false;
	$result['message'] = $ex->getMessage();
}

sleep(2);
echo json_encode($result);
die();
