<?
/**
*Functions related to Song
*/
include __DIR__."/../class/Database.php";
include __DIR__."/../includes/variables.php";

class SongHandler extends Database{

    function __construct() {
    }

    public function getArtistList($id) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT artist.id AS artistId,
                                            artist.name AS artistName
                                    FROM product_artist, artist
                                    WHERE product_artist.productId = ? AND product_artist.artistId = artist.id");
            $stmt->execute(array($id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $rows[] = $row;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getProductRating($id) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT AVG(rating) AS avg
                                    FROM rate
                                    WHERE productId = ?");
            $stmt->execute(array($id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $rows[] = $row;
            }
            return $rows[0];
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getDummyList($page) {
        global $SONG_PER_SEGMENT;
        $database = $this->connect_database();
        $current_time = strftime('%Y-%m-%d %H:%M:%S');
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            song.priceInTaka AS taka,
                                            song.priceInDollar AS dollar,
                                            downloadedView.downloadedTimes AS downloaded,
                                            song.isAlbum AS isAlbum,
                                            TIMESTAMPDIFF(MINUTE,song.uploadedOn,?) AS ago,
                                            song.bandId AS bandId,
                                            band.name AS bandName,
                                            song.albumId AS albumId,
                                            album.name AS albumName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN product AS album ON song.albumId = album.id
                                                            LEFT JOIN band ON song.bandId = band.id
                                                            LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT productId, COUNT(*) AS downloadedTimes
                                                                        FROM downloaded 
                                                                        GROUP BY productId)
                                                                      AS downloadedView ON downloadedView.productId = song.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    ORDER BY productId
                                    LIMIT ?");
            $stmt->execute(array($current_time, $page*$SONG_PER_SEGMENT));
            $rows = [];
            $i = 0;
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                if ($i >= ($page-1)*$SONG_PER_SEGMENT) {
                    $row['artistList'] = $this->getArtistList($row['productId']);
                    $rows[] = $row;
                }
                $i++;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getRecommended($userId, $page) {
        if($userId <= 1) {
            return $this->getDummyList($page);    
        }
    }

    public function getNew($page) {
        global $SONG_PER_SEGMENT;
        $database = $this->connect_database();
        $current_time = strftime('%Y-%m-%d %H:%M:%S');
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            song.priceInTaka AS taka,
                                            song.priceInDollar AS dollar,
                                            downloadedView.downloadedTimes AS downloaded,
                                            song.isAlbum AS isAlbum,
                                            TIMESTAMPDIFF(MINUTE,song.uploadedOn,?) AS ago,
                                            song.bandId AS bandId,
                                            band.name AS bandName,
                                            song.albumId AS albumId,
                                            album.name AS albumName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN product AS album ON song.albumId = album.id
                                                            LEFT JOIN band ON song.bandId = band.id
                                                            LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT productId, COUNT(*) AS downloadedTimes
                                                                        FROM downloaded 
                                                                        GROUP BY productId)
                                                                      AS downloadedView ON downloadedView.productId = song.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    ORDER BY ago ASC
                                    LIMIT ?");
            $stmt->execute(array($current_time, $page*$SONG_PER_SEGMENT));
            $rows = [];
            $i = 0;
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                if ($i >= ($page-1)*$SONG_PER_SEGMENT) {
                    $row['artistList'] = $this->getArtistList($row['productId']);
                    $rows[] = $row;
                }
                $i++;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getPopular($page) {
        global $SONG_PER_SEGMENT;
        $database = $this->connect_database();
        $current_time = strftime('%Y-%m-%d %H:%M:%S');
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            song.priceInTaka AS taka,
                                            song.priceInDollar AS dollar,
                                            downloadedView.downloadedTimes AS downloaded,
                                            song.isAlbum AS isAlbum,
                                            TIMESTAMPDIFF(MINUTE,song.uploadedOn,?) AS ago,
                                            song.bandId AS bandId,
                                            band.name AS bandName,
                                            song.albumId AS albumId,
                                            album.name AS albumName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN product AS album ON song.albumId = album.id
                                                            LEFT JOIN band ON song.bandId = band.id
                                                            LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT productId, COUNT(*) AS downloadedTimes
                                                                        FROM downloaded 
                                                                        WHERE TIMESTAMPDIFF(MINUTE,downloadTime,?) <= 10080
                                                                        GROUP BY productId)
                                                                      AS downloadedView ON downloadedView.productId = song.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    ORDER BY downloaded DESC
                                    LIMIT ?");
            $stmt->execute(array($current_time, $current_time, $page*$SONG_PER_SEGMENT));
            $rows = [];
            $i = 0;
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                if ($i >= ($page-1)*$SONG_PER_SEGMENT) {
                    $row['artistList'] = $this->getArtistList($row['productId']);
                    $rows[] = $row;
                }
                $i++;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getProduct($id) {
        $database = $this->connect_database();
        $current_time = strftime('%Y-%m-%d %H:%M:%S');
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            song.priceInTaka AS taka,
                                            song.priceInDollar AS dollar,
                                            downloadedViewThisWeek.downloadedTimes AS downloadedThisWeek,
                                            downloadedView.downloadedTimes AS downloaded,
                                            song.isAlbum AS isAlbum,
                                            TIMESTAMPDIFF(MINUTE,song.uploadedOn,?) AS ago,
                                            song.bandId AS bandId,
                                            band.name AS bandName,
                                            song.albumId AS albumId,
                                            album.name AS albumName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN product AS album ON song.albumId = album.id
                                                            LEFT JOIN band ON song.bandId = band.id
                                                            LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT productId, COUNT(*) AS downloadedTimes
                                                                        FROM downloaded 
                                                                        WHERE TIMESTAMPDIFF(MINUTE,downloadTime,?) <= 10080
                                                                        GROUP BY productId)
                                                                      AS downloadedViewThisWeek ON downloadedViewThisWeek.productId = song.id
                                                            LEFT JOIN (SELECT productId, COUNT(*) AS downloadedTimes
                                                                        FROM downloaded 
                                                                        GROUP BY productId)
                                                                      AS downloadedView ON downloadedView.productId = song.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    WHERE song.id = ?
                                    LIMIT 1");
            $stmt->execute(array($current_time, $current_time, $id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $row['artistList'] = $this->getArtistList($row['productId']);
                $rows[] = $row;
            }
            if (count($rows) == 0) {
                return 0;
            }
            else {
                return $rows;
            }
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getProductMinimum($id) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT id AS productId,
                                            name AS productName,
                                            priceInTaka AS taka,
                                            priceInDollar AS dollar,
                                            isAlbum
                                            FROM product
                                            WHERE id = ?
                                            LIMIT 1");
            $stmt->execute(array($id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $rows[] = $row;
            }
            if (count($rows) == 0) {
                return 0;
            }
            else {
                return $rows;
            }
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getProductFromSameAlbum($id) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    WHERE song.albumId = (SELECT album.albumId FROM product AS album WHERE album.id = ?)");
            $stmt->execute(array($id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $row['artistList'] = $this->getArtistList($row['productId']);
                $rows[] = $row;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }

    public function getProductFromAlbum($id) {
        $database = $this->connect_database();
        try {
            $stmt = $database->prepare("SELECT song.id AS productId,
                                            song.name AS productName,
                                            genre.name AS genre,
                                            rate.avg AS rating
                                    FROM product AS song    LEFT JOIN genre ON song.genreId = genre.id
                                                            LEFT JOIN (SELECT rate.productId, AVG(rating) AS avg
                                                                        FROM rate
                                                                        GROUP BY productId) AS rate ON rate.productId = song.id
                                    WHERE song.albumId = ?");
            $stmt->execute(array($id));
            $rows = [];
            while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                $row['artistList'] = $this->getArtistList($row['productId']);
                $rows[] = $row;
            }
            return $rows;
        } catch (PDOException $ex) {
            echo $ex->getMessage();
        }        
    }
}