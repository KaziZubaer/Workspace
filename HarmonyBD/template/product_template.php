<html>
<head>
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/bootstrap.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/common.css">
    <link rel="stylesheet" href="/HarmonyBD/css/navbar.css">
    <link rel="stylesheet" href="/HarmonyBD/css/footer.css">
    <link rel="stylesheet" href="/HarmonyBD/css/product.css">
</head>
<body>
    <?
    session_start();
    if(isset($_SESSION['user']))
        require_once __DIR__."/navbarIn_template.php";
    else
        require_once __DIR__."/navbarOut_template.php";
    ?>
    <div id="mainBody" style="background-image: url('img/lyrics/1.jpg')">
        <div class="divThird narrowDiv">
            <img src="img/commonIcon.png">
        </div>
        <div class="divThird wideDiv">
            <h2><strong><? echo $product['productName']?></strong><?echo $product['isAlbum'] ? "(Full Album)" : ""?></h2>
            <table>
                <tr>
                    <td>
                        <strong>Genre</strong>
                    </td>
                    <td>
                        <?echo $product['genre'] == NULL ? "Mixed" : $product['genre']?>
                    </td>
                </tr>
                <tr>
                    <td>
                        <strong>Singer</strong>
                    </td>
                    <td>
                        <strong>
                        <?
                        for ($i=0;$i<count($product['artistList']);$i++) {
                            echo $i==0 ? " ".$product['artistList'][$i]['artistName'] : ", ".$product['artistList'][$i]['artistName'];
                        }
                        ?>
                        </strong>
                    </td>
                </tr>
                <?
                if ($product['bandId'] != 0) {
                    echo 
                    "<tr>
                        <td>
                            <strong>Band</strong>
                        </td>
                        <td>
                            <strong>".$product['bandName']."</strong>
                        </td>
                    </tr>";
                }
                if ($product['albumId'] != 0) {
                    echo 
                    "<tr>
                        <td>
                            <strong>Album</strong>
                        </td>
                        <td>
                            <strong><a href='/HarmonyBD/product.php?id=".$product['albumId']."'>".$product['albumName']."</a></strong>
                        </td>
                    </tr>";
                }
                ?>
                <tr>
                    <td>
                        <strong>Download</strong>
                    </td>
                    <td>
                        <strong><?echo $product['downloadedThisWeek'] == NULL ? "0" : $product['downloadedThisWeek']?></strong>(this week), <strong><?echo $product['downloaded'] == NULL ? "0" : $product['downloaded']?></strong>(total)
                    </td>
                </tr>
                <tr>
                    <td>
                        <strong>Rating</strong> 
                    </td>
                    <td>
                        <div class="rating">
                            <?
                            $i=0;
                            $rating = $product['rating'] == NULL ? 0 : $product['rating'];
                            for(;$i<$rating;$i++) {
                                echo "<img src='img/golden_star.png'/>";
                            }
                            if ($i==0) {
                                echo "Not rated yet";
                            }
                            else {
                                for(;$i<5;$i++) {
                                    echo "<img src='img/gray_star.png'/>";
                                }
                            }
                            ?>
                        </div>
                        <div class="clear"></div>
                    </td>
                </tr>
                <tr>
                    <td>
                        <strong>Price</strong> 
                    </td>
                    <td>
                        <strong>৳<?echo $product['taka']?></strong> or <strong>$<?echo $product['dollar']?></strong>
                    </td>
                </tr>
                <tr>
                    <td>
                        <strong>Uploaded </strong><br/>            
                    </td>
                    <td>
                        <?
                        $ago = $product['ago'];
                        if ($ago <= 1) {
                            $ago = $ago.' minute ago';
                        }
                        else if ($ago <= 90) {
                            $ago = $ago.' minutes ago';
                        }
                        else if ($ago < 4320) {
                            $ago = round($ago/60).' hours ago';
                        }
                        else if ($ago < 525600) {
                            $ago = round($ago/1440).' days ago';
                        }
                        else if (Math.round($ago/525600) <= 1) {
                            $ago = round($ago/525600).' year ago';
                        }
                        else {
                            $ago = round($ago/525600).' years ago';
                        }
                        echo $ago;
                        ?>
                    </td>
                </tr>
            </table>
            <a href=<?echo "download.php?id=".$product['productId']?>><button><strong>Buy Now</strong></button></a>
        </div>
        <div class="clear"></div>
        <div id="commentSection"></div>
        <div id="isAlbum" style="display: none"><?echo $product['isAlbum']?></div>
        <div id="albumId" style="display: none"><?echo $product['albumId']?></div>
        <div id="productId" style="display: none"><?echo $product['productId']?></div>
        <div id="productName" style="display: none"><?echo $product['productName']?></div>
        <div id="moreSongAlbum" style="display: none">
            <?
            if ($product['isAlbum']) {
                echo $product['productName'];
            }
            else if ($product['albumId'] != 0) {
                echo $product['albumName'];   
            }
            ?>
        </div>
        <div id="moreSongs"></div>
    </div>
    <?require_once __DIR__."/footer_template.php";?>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
    <script src="/HarmonyBD/build/react.js"></script>
    <script src="/HarmonyBD/build/JSXTransformer.js"></script>
    <script src="/HarmonyBD/js/navbar.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/bootstrap.min.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/product_react.js"></script>
</body>
</html>
