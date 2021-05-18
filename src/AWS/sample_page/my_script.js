function callAwsLambdaFunction() {
	
	boat_id = document.getElementById('boat_id').value;
	dimension = document.getElementById('dimension').value;
	owner = document.getElementById('owner').value;

	// Send HTTP POST message to the API gateway using Ajax
    $.ajax({

    	// HTTP API gateway URL
        url: "https://zki9fxvfvi.execute-api.us-east-1.amazonaws.com/default/getReservation",
        type: 'POST',
        data: "Request",
        dataType: 'json',
        crossDomain: true,
        contentType: 'application/json',

        // Use custom headers to specify the type of request
        headers: {
	        "Access-Control-Allow-Methods": "*",
	        "Access-Control-Allow-Headers": "*",
	        "Access-Control-Allow-Origin": "*",
			"boat-id": boat_id,
			"boat-dim": dimension,
			"owner": owner
    	},

        // If response is received correctly 
        success: function(data) {
			console.log("Reservation done!");
        },
        // In case of error report failure
        error: function(data) {
		   	console.log("Error in reservation");
        }
    });
}