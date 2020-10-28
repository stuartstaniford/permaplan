// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for managing collecting mobile device geolocation data collection and upload.

// ========================================================================================
// Global Variables

var gStartTime  = 0.0;    // Time of first callback in ms.
var resultPara;           //Place to report what happened.


// ========================================================================================
// Possibly useful code to steal

/*
    console.log("i: " + i + " x: " + vectorPath[i][0] + " y: " + vectorPath[i][1]);

  if(gStartTime == 0.0)
   {
    gStartTime = (new Date()).getTime();
    now = gStartTime;
   }
  else
    now = (new Date()).getTime()
*/


// ========================================================================================
// Handle Errors in geolocation

function showGeoLocationError(error)
{
  switch(error.code)
   {
    case error.PERMISSION_DENIED:
      x.innerHTML = "User denied the request for Geolocation."
      break;
    case error.POSITION_UNAVAILABLE:
      x.innerHTML = "Location information is unavailable."
      break;
    case error.TIMEOUT:
      x.innerHTML = "The request to get user location timed out."
      break;
    case error.UNKNOWN_ERROR:
      x.innerHTML = "An unknown error occurred."
      break;
  }
}


// ========================================================================================
// Main entrypoint code for starting on page load

function handlePosition(position) {
  resultPara.innerHTML += "Latitude: " + position.coords.latitude +
  "<br>Longitude: " + position.coords.longitude + "<br><br>";
}



// ========================================================================================
// Main entrypoint code when the button is pressed

function recordNewTree()
{
  resultPara = document.getElementById("treeResultsPara");
  if (navigator.geolocation)
   {
    navigator.geolocation.getCurrentPosition(handlePosition, showGeoLocationError);
   }
  else
   {
    resultPara.innerHTML = "Geolocation is not supported by this browser.";
   }
}


// ========================================================================================
