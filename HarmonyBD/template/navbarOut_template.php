<html>
	<div id="signInModal" class="modal fade" role="dialog">
	  	<div class="modal-dialog">
	    <div class="modal-content">
	    	<div class="modal-header">
		        <h3 class="modal-title">Sign In</h3>
		    </div>
	     	<div class="modal-body">
	        	<form action="api/signIn.php" method="POST">
	        		Username: <br/>
	        		<input class="modalInput" type="text" name="username" pattern="[A-Za-z0-9]+" required autofocus></input> <br/>
	        		Password: <br/>
	        		<input class="modalInput" type="password" name="password" required></input> <br/>
	        		<a href="api/dummy.php">Forgot Password</a> <br/>
	        		<input type="hidden" name="location" value= <?echo $_SERVER['REQUEST_URI']?> ></input>
	        		<input type="submit" value="Sign In" class="modalSubmit"></input>
	        	</form>
	    	</div>
	    	</div>
	  	</div>
	</div>
	<div id="signUpModal" class="modal fade" role="dialog">
	  	<div class="modal-dialog">
	    <div class="modal-content">
	    	<div class="modal-header">
		        <h3 class="modal-title">Sign Up</h3>
		    </div>
	     	<div class="modal-body">
	        	<form id="signUpForm" name="signUpForm" action="api/addUser.php" method="POST">
	        		Username (Use maximum 10 letters and digits): <br/>
	        		<input class="modalInput" type="text" name="username" pattern="[A-Za-z0-9]+" maxlength="10" required autofocus></input> <br/>
	        		Full name: <br/>
	        		<input class="modalInput" type="text" name="name" pattern="[A-Z a-z]+" maxlength="40" required></input> <br/>
	        		Email address: <br/>
	        		<input class="modalInput" type="email" name="email" maxlength="40" required></input> <br/>
	        		Password (The minimum length should be 6): <br/>
	        		<input class="modalInput" type="password" name="password" minlength="6" required></input> <br/>
	        		*All fields are required <br/>
	        		<input type="hidden" name="location" value= <?echo $_SERVER['REQUEST_URI']?> ></input>
	        	</form>
	        	<button id="submit" class="modalSubmit">Sign Up</button>
	    	</div>
	    	</div>
	  	</div>
	</div>
	<div class="topNavbar">
		<div id="navbarName">
			<a href="/HarmonyBD">Harmony</a>
		</div>
		<div class="navbarOption" id="signIn" data-toggle="modal" data-target="#signInModal">
			Sign In
		</div>
		<div class="vline"></div>
		<div class="navbarOption" id="signIn" data-toggle="modal" data-target="#signUpModal">
			Sign Up
		</div>
	</div>
</html>