<?
include __DIR__."/../includes/functions.php";
include __DIR__."/../class/User.php";

$username = "";
$result = array(
	'success' => true, 
	'message' => '',
	'result' => false
);

if ($_SERVER["REQUEST_METHOD"] == "POST") {
	$username = test_input($_POST["username"]);
}

if ($username == "") {
	echo json_encode($result);
	die();
}

$user = new User();

try {
	$result['result'] = $user->checkUsername($username);
} catch (Exception $ex) {
	echo $ex->getMessage();
}

echo json_encode($result);
die();
