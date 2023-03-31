var MAX_USERNAME_LEN = 32;
var PWD_MIN_SIZE = 8;

function validateForm() 
 {
  var uname = document.getElementsByName("uname")[0].value;
  var psw1 = document.getElementsByName("psw1")[0].value;
  var psw2 = document.getElementsByName("psw2")[0].value;

  if (!isValidUsername(uname)) 
   {
    alert("Invalid username. Usernames must start with an alphabet character, be alphanumeric or contain an underscore (_), and be less than " + MAX_USERNAME_LEN + " characters.");
    return false;
  }

  if (!isValidPassword(psw1)) 
   {
    alert("Invalid password. Passwords must be at least " + PWD_MIN_SIZE + " characters long, contain at least one uppercase letter, one lowercase letter, one digit, and one special character.");
    return false;
   }

  if (psw1 !== psw2) 
   {
    alert("Passwords do not match. Please re-enter your passwords.");
    return false;
   }

  return true;
 }

function isValidUsername(username) 
 {
  var usernameRegex = /^[a-z][a-z0-9_]*$/i;

  return username.length < MAX_USERNAME_LEN && usernameRegex.test(username);
 }

function isValidPassword(password) 
 {
  var upper = /[A-Z]/;
  var lower = /[a-z]/;
  var digit = /[0-9]/;
  var symbol = /[!@#$%^&*(),.?":{}|<>]/;

  return password.length >= PWD_MIN_SIZE &&
         upper.test(password) &&
         lower.test(password) &&
         digit.test(password) &&
         symbol.test(password);
}
