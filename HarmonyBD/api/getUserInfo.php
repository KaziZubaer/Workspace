<?
include __DIR__."/../includes/variables.php";
include __DIR__."/../class/User.php";

session_start();

if (!isset($_SESSION['user'])) {
	header("Location: ".$BASE);
	die();
}

$result = array(
	'success' => true, 
	'message' => '',
	'info' => []
);

$user = new User();

try {
	$result['info'] = $user->getInfo($_SESSION['user']);
	if ($result['info'] == 0) {
		header("Location: ".$BASE);
		die();		
	}
} catch (Exception $ex) {
	$result['success'] = false;
	$result['message'] = $ex->getMessage();
}

echo json_encode($result);
die();