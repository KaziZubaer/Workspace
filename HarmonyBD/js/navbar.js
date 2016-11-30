$(document).ready(function(){
	$("#signOut").click(function(){
		$.ajax({
  			url: "api/signOut.php",
		}).done(function() {
			location.reload();
		});
	});

	$("#submit").click(function() {
		$.ajax({
			url: "api/checkUsername.php",
			method: "POST",
			data: {
				username: document.signUpForm.username.value
			},
			dataType: 'json',
			success: function(result) {
	        			if(result['result'] == true) {
	        				$("#signUpForm").submit(); 
	        			}
	        			else {
	        				alert("Sorry, The username is taken");
	        			}
	        		},
	        error: function() {
	        			alert("Something went wrong!");
	        			console.log(result['message']);
	        		}
		});
	});
});

