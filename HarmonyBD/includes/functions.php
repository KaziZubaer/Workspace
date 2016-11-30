<?
function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
 	$data = htmlspecialchars($data);
  	return $data;
}

function encrypt_password($password) {
	$cost = 10;
	$salt = strtr(base64_encode(mcrypt_create_iv(16, MCRYPT_DEV_URANDOM)), '+', '.');
	$salt = sprintf("$2a$%02d$", $cost) . $salt;
	return crypt($password, $salt);
}

function getUserId() {
	if (isset($_SESSION['user'])) {
		return $_SESSION['user'];
	}
	else {
		return 0;
	}
}