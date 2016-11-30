<?
include __DIR__."/../includes/variables.php";

session_start();
session_destroy();
header("Location: ".$BASE);
die();
