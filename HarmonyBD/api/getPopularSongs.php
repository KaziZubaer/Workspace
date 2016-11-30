<?
include __DIR__."/../songHandler/SongHandler.php";

$result = array(
	'success' => true, 
	'message' => '',
	'products' => []
);

$songHandler = new SongHandler();

try {
	$result['products'] = $songHandler->getPopular($_GET['page']);
} catch (Exception $ex) {
	$result['success'] = false;
	$result['message'] = $ex->getMessage();
}

sleep(2);
echo json_encode($result);
die();