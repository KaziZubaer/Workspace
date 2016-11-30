<html>
<head>
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/bootstrap.min.css">
    <link rel="stylesheet" href="/HarmonyBD/css/common.css">
    <link rel="stylesheet" href="/HarmonyBD/css/navbar.css">
    <link rel="stylesheet" href="/HarmonyBD/css/footer.css">
    <link rel="stylesheet" href="/HarmonyBD/css/restricted.css">
</head>
<body>
    <? require_once __DIR__."/navbarOut_template.php"; ?>
    <div id="mainBody">
        <h3>Sorry! You must be signed in to see the requested page.<h3>
        <div class="alias" id="signInAlias" data-toggle="modal" data-target="#signInModal">Sign In</div>
        <div class="alias" id="signUpAlias" data-toggle="modal" data-target="#signUpModal">Sign Up</div>
    </div>
    <?require_once __DIR__."/footer_template.php";?>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
    <script src="/HarmonyBD/build/react.js"></script>
    <script src="/HarmonyBD/build/JSXTransformer.js"></script>
    <script src="/HarmonyBD/js/navbar.js"></script>
    <script type="text/jsx" src="/HarmonyBD/js/bootstrap.min.js"></script>
</body>
</html>
