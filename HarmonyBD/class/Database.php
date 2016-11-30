<?
/**
*Database connection handler
*/
class Database {

	function __construct() {
	}

	public function connect_database() {
		$database = new PDO('mysql:host=127.0.0.1;dbname=HarmonyBD;charset=utf8', 'root', '');
		$database->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$database->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		return $database;
	}
}

