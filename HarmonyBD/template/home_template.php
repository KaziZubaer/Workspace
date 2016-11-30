<html>
<head>
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
    <link href='http://fonts.googleapis.com/css?family=Lobster' rel='stylesheet' type='text/css'>
    <link href='http://fonts.googleapis.com/css?family=Bitter' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" href="/HarmonyBD/css/bootstrap.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/common.css">
    <link rel="stylesheet" href="/HarmonyBD/css/navbar.css">
    <link rel="stylesheet" href="/HarmonyBD/css/footer.css">
    <link rel="stylesheet" href="/HarmonyBD/css/home.css">
</head>
<body>
    <?
    session_start();
    if(isset($_SESSION['user']))
        require_once __DIR__."/navbarIn_template.php";
    else
        require_once __DIR__."/navbarOut_template.php";
    ?>
    <div id="banner">
        <div id="bannerText">
            Feel Music
        </div>
    </div>
    <div id="searchDiv">
        <div id="searchBox">
            <form action="api/dummy.php" method="GET">
                <input type="text" name="keyword" placeholder="Search for song"/>
                <input type="submit" value="Search"/>
            </form>
        </div>
    </div>
    <div id="mainBody">
        <div class="divThird narrowDiv">
            <h2><strong>Recommended</strong></h2>
            <div id="recommended"></div>
        </div>
        <div class="divThird wideDiv">
            <h2><strong>New Songs</strong></h2>
            <div id="newSongs"></div>
        </div>
        <div class="divThird narrowDiv">
            <h2><strong>Top Downloads</strong></h2>
            <div id="topChart"></div>
        </div>
        <div class="clear"></div>
    </div>
    <?require_once __DIR__."/footer_template.php";?>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
    <script src="/HarmonyBD/build/react.js"></script>
    <script src="/HarmonyBD/build/JSXTransformer.js"></script>
    <script src="/HarmonyBD/js/navbar.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/bootstrap.min.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/home_react.js"></script>
</body>
</html>
