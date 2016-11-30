<?
include __DIR__."/../includes/variables.php";
include __DIR__."/../includes/functions.php";
include __DIR__."/../class/User.php";

session_start();
$username = $name = $password = $email = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
	$username = test_input($_POST["username"]);
	$name = test_input($_POST["name"]);
	$email = test_input($_POST["email"]);
	$password = test_input($_POST["password"]);
}

$password = encrypt_password($password);

$user = new User();

try {
	if ($user->checkUsername($username)) {
		$user->addUser($username, $name, $password, $email);
		$_SESSION['user'] = $username;
		if (isset($_POST['location'])) {
			header("Location: ".$_POST['location']);	
		}
		else {
			header("Location: ".$BASE);		
		}
	}
	die();
} catch (Exception $ex) {
	echo $ex->getMessage();
}
