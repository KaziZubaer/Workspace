<?
include __DIR__."/../includes/variables.php";
include __DIR__."/../includes/functions.php";
include __DIR__."/../class/User.php";

session_start();
$username = $password = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
	$username = test_input($_POST["username"]);
	$password = test_input($_POST["password"]);
}

$user = new User();

try {
	$result = $user->checkSignIn($username, $password);
	if ($result > 0) {
		$_SESSION['user'] = $username;
	}
	if (isset($_POST['location'])) {
		header("Location: ".$_POST['location']);	
	}
	else {
		header("Location: ".$BASE);		
	}
	die();
} catch (Exception $ex) {
	echo $ex->getMessage();
}
