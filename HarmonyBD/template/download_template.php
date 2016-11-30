<html>
<head>
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/bootstrap.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/common.css">
    <link rel="stylesheet" href="/HarmonyBD/css/navbar.css">
    <link rel="stylesheet" href="/HarmonyBD/css/footer.css">
    <link rel="stylesheet" href="/HarmonyBD/css/download.css">
</head>
<body>
    <?require_once __DIR__."/navbarIn_template.php";?>
    <div id="mainBody">
        <h3>You are about to buy <?echo $product['isAlbum'] ? "the album" : "the song"?> <strong><a href=<?echo "'/HarmonyBD/product.php?id=".$product['productId']."'"?>><?echo $product['productName']?></a></strong></h3><br/>
        <p>You can try at most <strong>3</strong> times to download the song within <strong>15</strong> minutes of your first attempt.</p>
        Check you desired option and click on download.<br/>
        <input type="radio" name="paymentOption" value="taka">Pay using balance in BDT (৳<?echo $product['taka']?>)</input><br/>
        <input type="radio" name="paymentOption" value="dollar">Pay using balance in USD ($<?echo $product['dollar']?>)</input><br/>
        <button id="confirm">Pay & Download</button>
    </div>
    <?require_once __DIR__."/footer_template.php";?>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
    <script src="/HarmonyBD/build/react.js"></script>
    <script src="/HarmonyBD/build/JSXTransformer.js"></script>
    <script src="/HarmonyBD/js/navbar.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/bootstrap.min.js"></script>
</body>
</html>
