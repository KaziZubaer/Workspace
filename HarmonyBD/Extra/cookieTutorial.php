<?
if (isset($_COOKIE["name"])) {
	echo "cookie all set";
}
else {
	echo "cookie is being set";
	setcookie("name", "value", time() + 10, "/");
}