var MAX_USERNAME_LEN = 32;
var PWD_MIN_SIZE = 8;


// =======================================================================================
/// @brief Sanity check the create new account form 

function validateCreateForm() 
 {
  var uname = document.getElementsByName("uname")[0].value;
  var psw1 = document.getElementsByName("psw1")[0].value;
  var psw2 = document.getElementsByName("psw2")[0].value;

  if (!isValidUsername(uname)) 
   {
    alert("Invalid username. Usernames must start with an alphabet character, be alphanumeric or contain an underscore (_), and be less than " + MAX_USERNAME_LEN + " characters.");
    return false;
  }

  return validatePasswordForm(psw1, psw2);
 }


// =======================================================================================
/// @brief Sanity check the change password form 

function validateChangeForm() 
{
  var oldPsw = document.getElementsByName("oldpsw")[0].value;
  var newPsw1 = document.getElementsByName("psw1")[0].value;
  var newPsw2 = document.getElementsByName("psw2")[0].value;

  if (oldPsw === "") {
    alert("Please enter your old password.");
    return false;
  }

  return validatePasswordForm(newPsw1, newPsw2);
}


// =======================================================================================
/// @brief Validate a password form with two fields (eg changing or creating a password).

function validatePasswordForm(psw1, psw2) 
{
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


// =======================================================================================
/// @brief Validate a username

function isValidUsername(username) 
 {
  var usernameRegex = /^[a-z][a-z0-9_]*$/i;

  return username.length < MAX_USERNAME_LEN && usernameRegex.test(username);
 }


// =======================================================================================
/// @brief Password complexity conditions.

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


// =======================================================================================
