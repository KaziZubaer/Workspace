<?
/**
*Functions related to Comment
*/
include __DIR__."/Database.php";
include __DIR__."/../includes/variables.php";

class Comment extends Database{

    function __construct() {
    }

    public function getComments($id, $page) {
        global $COMMENT_PER_SEGMENT;
        $database = $this->connect_database();
        $current_time = strftime('%Y-%m-%d %H:%M:%S');
        try {
            $stmt = $database->prepare("SELECT comment.comment AS commentText,
                                            user.id AS userId,
                                            user.name AS userName,
                                            TIMESTAMPDIFF(MINUTE,comment.commentedOn,?) AS ago,
                                            rate.rating AS rating
                                    FROM comment    LEFT JOIN user ON comment.userId = user.id
                                                    LEFT JOIN rate ON rate.productId = comment.productId
                                    WHERE comment.productId = ?
                                    ORDER BY ago ASC
                                    LIMIT ?");
            $stmt->execute(array($current_time, $id, $page*$COMMENT_PER_SEGMENT));
            $rows = [];
            $i = 0;
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                if ($i >= ($page-1)*$COMMENT_PER_SEGMENT) {
                    $rows[] = $row;
                }
                $i++;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }
}