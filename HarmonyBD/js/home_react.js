var Song = React.createClass({

	render: function() {
		var from = this.props.albumId === 0 ? [] : ["From ", <strong><a href={"/HarmonyBD/product.php?id="+this.props.albumId}>{this.props.albumName}</a></strong>, <br/>];
		if(this.props.bandId === 0) {
			var by = ["By"];
			for(var i=0;i<this.props.artistList.length;i++) {
				by.push(i==0 ? <strong> {this.props.artistList[i]['artistName']}</strong> : <strong>, {this.props.artistList[i]['artistName']}</strong>);
			}
			by.push(<br/>);
		}
		else {
			var by = ["By ", <strong>{this.props.bandName}</strong>, <br/>];
		}
		var tk = "৳";
		var thisWeek = this.props.thisWeek ? " this week" : "";
		var downloaded = !this.props.downloaded ? 0 : this.props.downloaded;
		var times = downloaded>1 ? "times" : "time";
		var ratingDiv = [];
		var rating = !this.props.rating ? 0 : this.props.rating;
		for (i=0;i<rating;i++) {
			ratingDiv.push(<img src="img/golden_star.png"/>);
		}
		if (i!=0) {
        	for (;i<5;i++) {
				ratingDiv.push(<img src="img/gray_star.png"/>);
			}
        }
		return(
			<div className="product">
				<table>
					<tr>
						<td>
							<img src="img/commonIcon.png" />
						</td>
						<td><small>
							<h5><strong><a href={"/HarmonyBD/product.php?id="+this.props.songId}>{this.props.songName}</a></strong></h5>
							<div className="rating">
								{ratingDiv}
							</div>
							<div className="clear"></div>
							{this.props.genre} genre<br/>
							{by}
							{from}
							Downloaded <strong>{downloaded}</strong> {times}{thisWeek}<br/>
							<strong>{tk}{this.props.taka}</strong> or <strong>${this.props.dollar}</strong><br/>
							<div className="greyText">{this.props.ago}</div>
						</small></td>
					</tr>
				</table>
			</div>
		);
	}
});

var Album = React.createClass({
	
	render: function() {
		if(this.props.bandId === 0) {
			var by = ["By"];
			for(var i=0;i<this.props.artistList.length;i++) {
				by.push(i==0 ? <strong> {this.props.artistList[i]['artistName']}</strong> : <strong>, {this.props.artistList[i]['artistName']}</strong>);
			}
			by.push(<br/>);
		}
		else {
			var by = ["By ", <strong>{this.props.bandName}</strong>, <br/>];
		}
		var tk = "৳";
		var thisWeek = this.props.thisWeek ? " this week" : "";
		var downloaded = !this.props.downloaded ? 0 : this.props.downloaded;
		var times = downloaded>1 ? "times" : "time";
		var ratingDiv = [];
		var rating = !this.props.rating ? 0 : this.props.rating;
		var i;
		for (i=0;i<rating;i++) {
			ratingDiv.push(<img src="img/golden_star.png"/>);
		}
		if (i!=0) {
        	for (;i<5;i++) {
				ratingDiv.push(<img src="img/gray_star.png"/>);
			}
        }
		return(
			<div className="product">
				<table>
					<tr>
						<td>
							<img src="img/commonIcon.png" />
						</td>
						<td><small>
							<h5><strong><a href={"/HarmonyBD/product.php?id="+this.props.albumId}>{this.props.albumName}</a> (Full Album)</strong></h5>
							<div className="rating">
								{ratingDiv}
							</div>
							<div className="clear"></div>
							{this.props.genre} genre<br/>
							{by}
							Downloaded <strong>{downloaded}</strong> {times}{thisWeek}<br/>
							<strong>{tk}{this.props.taka}</strong> or <strong>${this.props.dollar}</strong><br/>
							<div className="greyText">{this.props.ago}</div>
						</small></td>
					</tr>
				</table>
			</div>
		);
	}
});

var VerticalList = React.createClass({

	getInitialState: function() {
		return ({
			page: 1,
			productList: [],
			loading: false
		});
	},

	loadProducts: function(step) {
		
		this.setState({
			productList: <img className="center-block loading" src="img/loading.gif"/>,
			loading: true
		});
		$.ajax({
			url: "api/"+this.props.api,
			method: "GET",
			data: {
				page: this.state.page+step
			},
			dataType: 'json',
			success: function(result) {
            			tempProductList = [];
            			if (result['products'].length === 0) {
            				this.setState ({
								page: this.state.page+step,
								loading: false
	            			});
            				this.goPrev();
            			}
            			else {
	            			for (var i=0;i<result['products'].length;i++) {
								var ago = result['products'][i]['ago'];
								if (ago <= 0) {
									ago = '0 minute ago';
								}
								else if (ago <= 1) {
									ago = '1 minute ago';
								}
								else if (ago <= 90) {
									ago = ago + ' minutes ago';
								}
								else if (ago < 4320) {
									ago = Math.round(ago/60) + ' hours ago';
								}
								else if (ago < 525600) {
									ago = Math.round(ago/1440) + ' days ago';
								}
								else if (Math.round(ago/525600) <= 1) {
									ago = Math.round(ago/525600) + ' year ago';
								}
								else {
									ago = Math.round(ago/525600) + ' years ago';
								}
								if (this.props.api === "getPopularSongs.php") {
									var thisWeek = true;
								}
								else {
									var thisWeek = false;
								}
								if (result['products'][i]['isAlbum'] === 1) {
									tempProductList.push(<Album albumId={result['products'][i]['productId']} albumName={result['products'][i]['productName']} taka={result['products'][i]['taka']} dollar={result['products'][i]['dollar']} downloaded={result['products'][i]['downloaded']} genre={result['products'][i]['genre']} ago={ago} artistList={result['products'][i]['artistList']} bandId={result['products'][i]['bandId']} bandName={result['products'][i]['bandName']} thisWeek={thisWeek} rating={result['products'][i]['rating']}/>);
								}
								else {
									tempProductList.push(<Song songId={result['products'][i]['productId']} songName={result['products'][i]['productName']} taka={result['products'][i]['taka']} dollar={result['products'][i]['dollar']} downloaded={result['products'][i]['downloaded']} genre={result['products'][i]['genre']} ago={ago} artistList={result['products'][i]['artistList']} bandId={result['products'][i]['bandId']} bandName={result['products'][i]['bandName']} albumId={result['products'][i]['albumId']} albumName={result['products'][i]['albumName']} thisWeek={thisWeek} rating={result['products'][i]['rating']}/>);
								}
							}
							this.setState ({
								page: this.state.page+step,
	            				productList: tempProductList,
	            				loading: false
	            			});
						}
            			console.log(result['message']);
            		}.bind(this),
            error: function() {
            			this.setState ({
	            			loading: false
	            		});
            			console.log("Song load error!");
            		}.bind(this)
    	});
	},

	componentWillMount: function() {
		this.loadProducts(0);
	},

	goPrev: function() {
		if (this.state.page>1 && !this.state.loading) {
			this.loadProducts(-1);
		}
	},

	goNext: function() {
		if (!this.state.loading) {
			this.loadProducts(1);
		}
	},

	render: function() {
		return(
			<div>
				<div className="pagination">
					<div className="shadowButton" onClick={this.goPrev}><i className="fa fa-angle-left fa-4x"></i></div>
					<div className="pageNumber">{this.state.page}</div>
					<div className="shadowButton" onClick={this.goNext}><i className="fa fa-angle-right fa-4x"></i></div>
				</div>
				<div className="clear"></div>
				{this.state.productList}
			</div>
		);
	}
});

$(document).ready(function(){
    React.render(<VerticalList api="getRecommendedSongs.php"/>, document.getElementById('recommended'));
    React.render(<VerticalList api="getNewSongs.php"/>, document.getElementById('newSongs'));
    React.render(<VerticalList api="getPopularSongs.php"/>, document.getElementById('topChart'));
});
