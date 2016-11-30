Personal = React.createClass ({

	getInitialState: function() {
		return ({
			name: "",
			email: ""
		});
	},

	componentWillMount: function() {
		$.ajax({
			url: "api/getUserInfo.php",
			dataType: 'json',
			success: function(result) {
							this.setState({
								name: result['info']['name'],
								email: result['info']['email']
							});
            				console.log(result['message']);
            			}.bind(this),
            error: function() {
            				console.log("Information load error!");
            			}.bind(this)
    	});
	},

	render: function() {
		return (
			<div>
				<h2><strong>{this.state.name}</strong></h2><br/>
				<form id="updateForm" name="updateForm" action="" method="POST">
					<small><strong>Email Address: </strong></small><br/>
					<input className="profileInputBox" type="email" name="email" maxlength="40" required></input><br/>
					<small><strong>Change Password: </strong></small><br/>
					<input className="profileInputBox" type="password" name="passwordNew"></input><br/>
					<small><strong>Existing Password: </strong></small><br/>
					<input className="profileInputBox" type="password" name="passwordOld" required></input><br/>
	        		<input type="hidden" name="location" value="/profile.php"></input>
	        	</form>
	        	<button id="updateButton">Update</button>
			</div>
		);
	}
});

$(document).ready(function(){
	React.render(<Personal />, document.getElementById('personal'));
});
