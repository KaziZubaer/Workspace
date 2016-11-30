<?
require_once __DIR__."/../class/Comment.php";

$result = array(
	'success' => true, 
	'message' => '',
	'comments' => []
);

$comment = new Comment();

try {
	$result['comments'] = $comment->getComments($_GET['id'], $_GET['page']);
} catch (Exception $ex) {
	$result['success'] = false;
	$result['message'] = $ex->getMessage();
}

sleep(2);
echo json_encode($result);
die();
