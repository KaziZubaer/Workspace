<?
include __DIR__."/../includes/functions.php";
include __DIR__."/../songHandler/SongHandler.php";

session_start();
$userId = getUserId();

$result = array(
	'success' => true, 
	'message' => '',
	'products' => []
);

$songHandler = new SongHandler();

try {
	$result['products'] = $songHandler->getRecommended($userId, $_GET['page']);
} catch (Exception $ex) {
	$result['success'] = false;
	$result['message'] = $ex->getMessage();
}

sleep(2);
echo json_encode($result);
die();