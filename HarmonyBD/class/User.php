<?
/**
*Functions related to users 
*/
include __DIR__."/Database.php";

class User extends Database{

	function __construct() {
	}

	public function addUser($username, $name, $password, $email) {
		$database = $this->connect_database();
		try {
        	$stmt = $database->prepare("INSERT INTO user (username, name, password, email) VALUES (?, ?, ?, ?)");
        	$stmt->execute(array($username, $name, $password, $email));
        } catch (PDOException $ex) {
			echo $ex->getMessage();
		}
	}

    public function checkSignIn($username, $password) {
		$database = $this->connect_database();
		try {
        	$stmt = $database->prepare("SELECT id, password FROM user WHERE username = ? LIMIT 1");
        	$stmt->execute(array($username));
        	$userCount = $stmt->rowCount();

        	if ($userCount == 0) {
                return 0;
        	}
        	$returnValue = -1;
        	while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                if (hash_equals($row['password'], crypt($password, $row['password']))) {
        			$returnValue = $row['id'];
                }
        	}
        	return $returnValue;
        } catch (PDOException $ex) {
			echo $ex->getMessage();
		}
	}

    public function checkUsername($username) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT id FROM user WHERE username = ?");
            $stmt->execute(array($username));
            $userCount = $stmt->rowCount();
            if ($userCount == 0) {
                return true;
            }
            else {
                return false;
            }
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }
    }

    public function getInfo($username) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT name, email, dollar, taka FROM user WHERE username = ? LIMIT 1");
            $stmt->execute(array($username));
            $userCount = $stmt->rowCount();

            if ($userCount == 0) {
                return 0;
            }

            else {
                $row = $stmt->fetch(PDO::FETCH_ASSOC);
                return $row;
            }
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }
    }   
}