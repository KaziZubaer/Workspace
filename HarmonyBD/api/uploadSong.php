<?
include __DIR__."/connectDB.php";

$name = $_FILES["song"]["name"];
$tmp_name = $_FILES["song"]["tmp_name"];
$type = $_FILES["song"]["type"];
$error = $_FILES["song"]["error"];


	global $database;

	if (file_exists("songs/".$name)) {
		echo "File exists";
	}
	else {
		$location = "songs/".$name;
		move_uploaded_file($tmp_name, $location);
		try{
        	$stmt = $database->prepare("INSERT INTO song (location) VALUES (?)");
        	$stmt->execute(array($location));
        } catch (PDOException $ex) {
			echo $ex->getMessage();
		}
		echo "<a href='$location'>Download</a>";
	}
