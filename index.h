const char LOGIN_PAGE[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Smart Alarm System - Login</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #f0f8ff;
        margin: 0;
        padding: 0;
        display: flex;
        flex-direction: column;
        min-height: 100vh;
      }
      
      h1 {
        color: #FF8C00;
        text-align: center;
        margin-top: 20px;
        text-decoration: underline;
      }
      
      .login-container {
        background-color: #333;
        color: #FF8C00;
        border-radius: 8px;
        padding: 30px;
        max-width: 400px;
        width: 90%;
        margin: 30px auto;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      }
      
      .form-group {
        margin-bottom: 15px;
        text-align: left;
      }
      
      .form-group label {
        display: block;
        margin-bottom: 5px;
      }
      
      .form-group input {
        width: 100%;
        padding: 10px;
        border: 1px solid #ddd;
        border-radius: 4px;
        box-sizing: border-box;
      }
      
      .submit-btn {
        background-color: #FF8C00;
        color: white;
        border: none;
        padding: 10px 15px;
        border-radius: 4px;
        cursor: pointer;
        font-size: 16px;
        width: 100%;
        margin-top: 10px;
      }
      
      .submit-btn:hover {
        background-color: #e07b00;
      }
    </style>
  </head>
  <body>
    <h1>IOT Based Smart Alarm System</h1>
    
    <div class="login-container">
      <h2>LOGIN FORM</h2>
      <form method="POST" action="/login">
        <div class="form-group">
          <label for="username">USERNAME</label>
          <input type="text" id="username" name="USERNAME" placeholder="User Name" required>
        </div>
        
        <div class="form-group">
          <label for="password">PASSWORD</label>
          <input type="password" id="password" name="PASSWORD" placeholder="Password" required>
        </div>
        
        <input type="submit" class="submit-btn" name="SUBMIT" value="Login">
      </form>
    </div>
  </body>
  </html>
  )=====";
  
  const char ALARM_FORM[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Set Alarms</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin-top: 30px;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .alarm-container {
          display: flex;
          flex-wrap: wrap;
          justify-content: space-around;
        }
        
        .alarm-group {
          width: calc(50% - 20px);
          background-color: #e6f7ff;
          border-radius: 8px;
          padding: 15px;
          margin: 20px 0;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        
        .alarm-form {
          margin-bottom: 30px;
        }
        
        .time-inputs {
          display: flex;
          justify-content: center;
          gap: 10px;
          margin: 10px 0;
        }
        
        .time-inputs input {
          width: 55px !important;
          text-align: center;
          padding: 8px;
          border: 1px solid #ccc;
          border-radius: 4px;
        }
        
        .form-group {
          margin: 15px 0;
          text-align: center;
        }
        
        .button-group {
          display: flex;
          justify-content: center;
          gap: 10px;
          margin-top: 15px;
        }
        
        /* Friday-specific hidden alarms */
        #extraAlarms {
          width: 100%;
          display: flex;
          flex-wrap: wrap;
          justify-content: space-around;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      <hr>
      <hr>
    
      <!-- Day selection and copy controls -->
      <div class="day-selector">
      <h3>Select Day:</h3>
      <form>
        <select id="daySelector" name="DAY_SELECT" onchange="updateAlarmFields()">
          <option value="monday">Monday</option>
          <option value="tuesday">Tuesday</option>
          <option value="wednesday">Wednesday</option>
          <option value="thursday">Thursday</option>
          <option value="friday">Friday</option>
          <option value="saturday">Saturday</option>
          <option value="sunday">Sunday</option>
        </select>
      </div>
      
      <!-- Copy to all days button -->
      <form id="copyForm">
        <button type="button" id="copyButton" class="copy-button" onclick="confirmCopyToAllDays()">
          Copy Current Day's Alarms to All Days
        </button>
        <input type="hidden" name="COPY_FROM_DAY" id="copyFromDay" value="">
      </form>
    </div>
    
    <h2>SET ALARM</h2>
    
    <div id="alarmContainer" class="alarm-container">
      <!-- Alarms 1-4 (visible for all days) -->
      <div class="alarm-group">
        <form class="alarm-form" method="POST" action="/">
         <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
          <h3>Alarm No: 1</h3>
          <div class="form-group">
            <label for="alarm1Name">Alarm Name:</label>
            <input type="text" id="alarm1Name" name="NAME_ALARM1" placeholder="Alarm Name">
          </div>
          
          <div class="form-group">
            <label>Set Time:</label>
            <div class="time-inputs">
              <input type="number" name="HH_ALARM1" min="0" max="23" placeholder="HH">
              <input type="number" name="MM_ALARM1" min="0" max="59" placeholder="MM">
              <input type="number" name="SS_ALARM1" min="0" max="59" placeholder="SS">
            </div>
          </div>
          
          <div class="button-group">
            <input type="submit" name="SUBMIT" value="Set" class="set-btn">
            <button type="reset" class="clear-btn">Clear</button>
          </div>
        </form>
        
        <form class="alarm-form" method="POST" action="/">
         <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
          <h3>Alarm No: 2</h3>
          <div class="form-group">
            <label for="alarm2Name">Alarm Name:</label>
            <input type="text" id="alarm2Name" name="NAME_ALARM2" placeholder="Alarm Name">
          </div>
          
          <div class="form-group">
            <label>Set Time:</label>
            <div class="time-inputs">
              <input type="number" name="HH_ALARM2" min="0" max="23" placeholder="HH">
              <input type="number" name="MM_ALARM2" min="0" max="59" placeholder="MM">
              <input type="number" name="SS_ALARM2" min="0" max="59" placeholder="SS">
            </div>
          </div>
          
          <div class="button-group">
            <input type="submit" name="SUBMIT" value="Set" class="set-btn">
            <button type="reset" class="clear-btn">Clear</button>
          </div>
        </form>
      </div>
      
      <div class="alarm-group">
        <form class="alarm-form" method="POST" action="/">
         <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
          <h3>Alarm No: 3</h3>
          <div class="form-group">
            <label for="alarm3Name">Alarm Name:</label>
            <input type="text" id="alarm3Name" name="NAME_ALARM3" placeholder="Alarm Name">
          </div>
          
          <div class="form-group">
            <label>Set Time:</label>
            <div class="time-inputs">
              <input type="number" name="HH_ALARM3" min="0" max="23" placeholder="HH">
              <input type="number" name="MM_ALARM3" min="0" max="59" placeholder="MM">
              <input type="number" name="SS_ALARM3" min="0" max="59" placeholder="SS">
            </div>
          </div>
          
          <div class="button-group">
            <input type="submit" name="SUBMIT" value="Set" class="set-btn">
            <button type="reset" class="clear-btn">Clear</button>
          </div>
        </form>
        
        <form class="alarm-form" method="POST" action="/">
         <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
          <h3>Alarm No: 4</h3>
          <div class="form-group">
            <label for="alarm4Name">Alarm Name:</label>
            <input type="text" id="alarm4Name" name="NAME_ALARM4" placeholder="Alarm Name">
          </div>
          
          <div class="form-group">
            <label>Set Time:</label>
            <div class="time-inputs">
              <input type="number" name="HH_ALARM4" min="0" max="23" placeholder="HH">
              <input type="number" name="MM_ALARM4" min="0" max="59" placeholder="MM">
              <input type="number" name="SS_ALARM4" min="0" max="59" placeholder="SS">
            </div>
          </div>
          
          <div class="button-group">
            <input type="submit" name="SUBMIT" value="Set" class="set-btn">
            <button type="reset" class="clear-btn">Clear</button>
          </div>
        </form>
      </div>
      
      <!-- Alarms 5-8 (hidden for Friday) -->
      <div id="extraAlarms" class="alarm-container">
        <div class="alarm-group">
          <form class="alarm-form" method="POST" action="/">
           <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
            <h3>Alarm No: 5</h3>
            <div class="form-group">
              <label for="alarm5Name">Alarm Name:</label>
              <input type="text" id="alarm5Name" name="NAME_ALARM5" placeholder="Alarm Name">
            </div>
            
            <div class="form-group">
              <label>Set Time:</label>
              <div class="time-inputs">
                <input type="number" name="HH_ALARM5" min="0" max="23" placeholder="HH">
                <input type="number" name="MM_ALARM5" min="0" max="59" placeholder="MM">
                <input type="number" name="SS_ALARM5" min="0" max="59" placeholder="SS">
              </div>
            </div>
            
            <div class="button-group">
              <input type="submit" name="SUBMIT" value="Set" class="set-btn">
              <button type="reset" class="clear-btn">Clear</button>
            </div>
          </form>
          
          <form class="alarm-form" method="POST" action="/">
           <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
            <h3>Alarm No: 6</h3>
            <div class="form-group">
              <label for="alarm6Name">Alarm Name:</label>
              <input type="text" id="alarm6Name" name="NAME_ALARM6" placeholder="Alarm Name">
            </div>
            
            <div class="form-group">
              <label>Set Time:</label>
              <div class="time-inputs">
                <input type="number" name="HH_ALARM6" min="0" max="23" placeholder="HH">
                <input type="number" name="MM_ALARM6" min="0" max="59" placeholder="MM">
                <input type="number" name="SS_ALARM6" min="0" max="59" placeholder="SS">
              </div>
            </div>
            
            <div class="button-group">
              <input type="submit" name="SUBMIT" value="Set" class="set-btn">
              <button type="reset" class="clear-btn">Clear</button>
            </div>
          </form>
        </div>
        
        <div class="alarm-group">
          <form class="alarm-form" method="POST" action="/">
           <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
            <h3>Alarm No: 7</h3>
            <div class="form-group">
              <label for="alarm7Name">Alarm Name:</label>
              <input type="text" id="alarm7Name" name="NAME_ALARM7" placeholder="Alarm Name">
            </div>
            
            <div class="form-group">
              <label>Set Time:</label>
              <div class="time-inputs">
                <input type="number" name="HH_ALARM7" min="0" max="23" placeholder="HH">
                <input type="number" name="MM_ALARM7" min="0" max="59" placeholder="MM">
                <input type="number" name="SS_ALARM7" min="0" max="59" placeholder="SS">
              </div>
            </div>
            
            <div class="button-group">
              <input type="submit" name="SUBMIT" value="Set" class="set-btn">
              <button type="reset" class="clear-btn">Clear</button>
            </div>
          </form>
          
          <form class="alarm-form" method="POST" action="/">
           <input type="hidden" name="DAY_SELECTED" id="daySelected" value="monday">
            <h3>Alarm No: 8</h3>
            <div class="form-group">
              <label for="alarm8Name">Alarm Name:</label>
              <input type="text" id="alarm8Name" name="NAME_ALARM8" placeholder="Alarm Name">
            </div>
            
            <div class="form-group">
              <label>Set Time:</label>
              <div class="time-inputs">
                <input type="number" name="HH_ALARM8" min="0" max="23" placeholder="HH">
                <input type="number" name="MM_ALARM8" min="0" max="59" placeholder="MM">
                <input type="number" name="SS_ALARM8" min="0" max="59" placeholder="SS">
              </div>
            </div>
            
            <div class="button-group">
              <input type="submit" name="SUBMIT" value="Set" class="set-btn">
              <button type="reset" class="clear-btn">Clear</button>
            </div>
          </form>
        </div>
      </div>
    </div>
    
    <script>
      function updateAlarmFields() {
  const day = document.getElementById('daySelector').value;
  const extraAlarms = document.getElementById('extraAlarms');
  
  // Update all hidden daySelected inputs
  document.querySelectorAll('[name="DAY_SELECTED"]').forEach(input => {
    input.value = day;
  });

  // Toggle extra alarms visibility
  extraAlarms.style.display = (day === 'friday') ? 'none' : 'flex';
}
      
      function confirmCopyToAllDays() {
        const currentDay = document.getElementById('daySelector').value;
        if (confirm(`Are you sure you want to copy all alarm settings from ${currentDay} to all other days?`)) {
          // This would submit the form to copy settings
          const form = document.getElementById('copyForm');
          
          // Create and append a hidden input for copy action
          const actionInput = document.createElement('input');
          actionInput.type = 'hidden';
          actionInput.name = 'COPY_TO_ALL_DAYS';
          actionInput.value = 'YES';
          form.appendChild(actionInput);
          
          // Submit the form
          form.method = 'POST';
          form.action = '/?COPY_ALARMS=YES';
          form.submit();
        }
      }
      
      // Initialize the form on page load
      document.addEventListener('DOMContentLoaded', function() {
        updateAlarmFields();
      });
    </script>
    
    <div class="nav-links">
      <a href="/">HOME</a>
      <a href="/login?LOGOUT=YES">Log Out</a>
    </div>
  </body>
  </html>
  )=====";
  
  const char STATUS_PAGE[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Smart Alarm System - Status</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: powderblue;
        margin: 0;
        padding: 0 15px;
      }
      
      h1, h2, h3 {
        color: #0066cc;
        text-align: center;
      }
      
      h2 {
        text-decoration: underline;
        margin-top: 30px;
      }
      
      hr {
        border: 0;
        height: 2px;
        background-color: #3399ff;
        width: 90%;
        margin: 10px auto;
      }
      
      .day-selector {
        text-align: center;
        margin: 20px 0;
      }
      
      .day-selector select {
        padding: 8px;
        font-size: 16px;
        border-radius: 4px;
      }
      
      table {
        width: 90%;
        max-width: 800px;
        margin: 20px auto;
        border-collapse: collapse;
        background-color: white;
        box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
        border-radius: 8px;
        overflow: hidden;
      }
      
      th, td {
        border: 1px solid #3399ff;
        padding: 12px;
        text-align: center;
      }
      
      th {
        background-color: #e6f7ff;
        color: #3333ff;
        font-weight: bold;
      }
      
      tr:nth-child(even) {
        background-color: #f9fdff;
      }
      
      .nav-links {
        margin-top: 30px;
        text-align: center;
      }
      
      .nav-links a {
        display: inline-block;
        color: #0066cc;
        text-decoration: none;
        margin: 0 10px;
      }
      
      .nav-links a:hover {
        text-decoration: underline;
      }
    </style>
  </head>
  <body>
    <h1>SMART ALARM SYSTEM</h1>
    
    <hr>
    <hr>
    
    <!-- Day selection dropdown -->
    <div class="day-selector">
      <h3>Select Day:</h3>
      <form>
        <select id="daySelector" name="DAY_SELECT" onchange="this.form.submit()">
          <option value="monday" @@MON_SELECTED@@>Monday</option>
          <option value="tuesday" @@TUE_SELECTED@@>Tuesday</option>
          <option value="wednesday" @@WED_SELECTED@@>Wednesday</option>
          <option value="thursday" @@THU_SELECTED@@>Thursday</option>
          <option value="friday" @@FRI_SELECTED@@>Friday</option>
          <option value="saturday" @@SAT_SELECTED@@>Saturday</option>
          <option value="sunday" @@SUN_SELECTED@@>Sunday</option>
        </select>
        <input type="hidden" name="ALARM_STATUS" value="YES">
      </form>
    </div>
    
    <h2>Today is: @@CURRENT_DAY@@</h2>
    <h2>Showing Alarms for: @@SELECTED_DAY@@</h2>
    
    <table>
      <tr>
        <th>Alarm Number</th>
        <th>Alarm Name</th>
        <th>Time</th>
      </tr>
      <tr>
        <td>1</td>
        <td>@@NAME1@@</td>
        <td>@@SET1@@</td>
      </tr>
      <tr>
        <td>2</td>
        <td>@@NAME2@@</td>
        <td>@@SET2@@</td>
      </tr>
      <tr>
        <td>3</td>
        <td>@@NAME3@@</td>
        <td>@@SET3@@</td>
      </tr>
      <tr>
        <td>4</td>
        <td>@@NAME4@@</td>
        <td>@@SET4@@</td>
      </tr>
    </table>
    
    <!-- Only show these for non-Friday days -->
    <div id="extraAlarmStatus" style="display: @@EXTRA_DISPLAY@@;">
      <table>
        <tr>
          <td>5</td>
          <td>@@NAME5@@</td>
          <td>@@SET5@@</td>
        </tr>
        <tr>
          <td>6</td>
          <td>@@NAME6@@</td>
          <td>@@SET6@@</td>
        </tr>
        <tr>
          <td>7</td>
          <td>@@NAME7@@</td>
          <td>@@SET7@@</td>
        </tr>
        <tr>
          <td>8</td>
          <td>@@NAME8@@</td>
          <td>@@SET8@@</td>
        </tr>
      </table>
    </div>
    
    <div class="nav-links">
      <a href="/">HOME</a>
      <a href="/login?LOGOUT=YES">Log Out</a>
    </div>
  </body>
  </html>
  )=====";
  
  const char HOME_PAGE[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Smart Alarm System - Home</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: powderblue;
        margin: 0;
        padding: 0 15px;
        display: flex;
        flex-direction: column;
        align-items: center;
      }
      
      h1, h2, h3 {
        color: #0066cc;
        text-align: center;
      }
      
      hr {
        border: 0;
        height: 2px;
        background-color: #3399ff;
        width: 90%;
        margin: 10px auto;
      }
      
      .button-container {
        display: flex;
        flex-direction: column;
        gap: 20px;
        width: 90%;
        max-width: 400px;
        margin: 30px auto;
      }
      
      .action-btn {
        background-color: #99cc00;
        color: white;
        border: none;
        padding: 15px;
        border-radius: 8px;
        cursor: pointer;
        font-size: 16px;
        text-transform: uppercase;
        font-weight: bold;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        transition: transform 0.2s, background-color 0.2s;
      }
      
      .action-btn:hover {
        background-color: #88bb00;
        transform: translateY(-2px);
      }
      
      .logout-link {
        margin-top: 30px;
        text-align: center;
      }
      
      .logout-link a {
        color: #0066cc;
        text-decoration: none;
      }
      
      .logout-link a:hover {
        text-decoration: underline;
      }
    </style>
  </head>
  <body>
    <h1>SMART ALARM SYSTEM</h1>
    
    <hr>
    <hr>
    
    <div class="button-container">
      <form action="/?ALARM=YES" method="POST">
        <input type="submit" class="action-btn" name="SUBMIT" value="SET ALARM">
      </form>
      
      <form action="/?ALARM_STATUS=YES" method="POST">
        <input type="submit" class="action-btn" name="SUBMIT" value="ALARM STATUS">
      </form>
      
      <form action="/?RESET_DATE=YES" method="POST">
        <input type="submit" class="action-btn" name="SUBMIT" value="SHOW & RESET TIME & DATE">
      </form>
      
      <form action="/?HOME_AUTOMATION=YES" method="POST">
        <input type="submit" class="action-btn" name="SUBMIT" value="MANUAL BUZZER CONTROL">
      </form>
    </div>
    
    <div class="logout-link">
      <a href="/login?LOGOUT=YES"><h3>Log Out</h3></a>
    </div>
  </body>
  </html>
  )=====";
  
  const char SHOW_DATE[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Date & Time</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .time-display {
          background-color: #e6f7ff;
          border-radius: 8px;
          padding: 20px;
          margin: 30px auto;
          max-width: 500px;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        
        .time-display p {
          color: #3333ff;
          font-size: 18px;
          margin: 15px 0;
        }
        
        .time-form {
          background-color: #e6f7ff;
          border-radius: 8px;
          padding: 20px;
          margin: 30px auto;
          max-width: 500px;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        
        .form-group {
          margin: 20px 0;
        }
        
        .form-group label {
          display: block;
          color: #3333ff;
          font-size: 18px;
          margin-bottom: 10px;
        }
        
        .time-inputs {
          display: flex;
          gap: 10px;
          justify-content: center;
        }
        
        .time-inputs input {
          width: 60px;
          text-align: center;
          padding: 8px;
          border: 1px solid #ccc;
          border-radius: 4px;
        }
        
        .set-btn {
          background-color: #99cc00;
          color: white;
          border: none;
          padding: 10px 20px;
          border-radius: 4px;
          cursor: pointer;
          font-size: 16px;
          display: block;
          margin: 20px auto 0;
        }
        
        .set-btn:hover {
          background-color: #88bb00;
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
        }
        
        .nav-links a:hover {
          text-decoration: underline;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="time-display">
        <p>Present Time: <strong>@@H@@:@@M@@:@@S@@</strong></p>
        <p>Present Date: <strong>@@D@@/@@MO@@/@@Y@@</strong></p>
        <p>Current Day: <strong>@@CURRENT_DAY@@</strong></p>
      </div>
      
      <form class="time-form" method="GET">
        <div class="form-group">
          <label for="timeInputs">Set Present Time:</label>
          <div class="time-inputs">
            <input type="number" id="hourInput" name="HH_PRE" min="0" max="23" placeholder="HH" required>
            <input type="number" id="minuteInput" name="MM_PRE" min="0" max="59" placeholder="MM" required>
            <input type="number" id="secondInput" name="SS_PRE" min="0" max="59" placeholder="SS" required>
          </div>
        </div>
        
        <div class="form-group">
          <label for="dateInputs">Set Present Date:</label>
          <div class="time-inputs">
            <input type="number" id="dayInput" name="DD_PRE" min="1" max="31" placeholder="DD" required>
            <input type="number" id="monthInput" name="MO_PRE" min="1" max="12" placeholder="MM" required>
            <input type="number" id="yearInput" name="YYYY_PRE" min="2023" max="2099" placeholder="YYYY" required>
          </div>
        </div>
        
        <input type="submit" name="SET_PRE_DATE" value="Set Date & Time" class="set-btn">
      </form>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
    const char APPLIANCES[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Smart Alarm System - Control Buzzer</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: powderblue;
      margin: 0;
      padding: 0 15px;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    
    h1, h2, h3 {
      color: #0066cc;
      text-align: center;
    }
    
    hr {
      border: 0;
      height: 2px;
      background-color: #3399ff;
      width: 90%;
      margin: 10px auto;
    }
    
    .appliance-controls {
      display: flex;
      flex-direction: column;
      gap: 20px;
      width: 90%;
      max-width: 400px;
      margin: 50px auto;
    }
    
    .control-btn {
      padding: 15px;
      border-radius: 8px;
      cursor: pointer;
      font-size: 18px;
      font-weight: bold;
      border: none;
      text-align: center;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      transition: transform 0.2s, box-shadow 0.2s;
    }
    
    .on-btn {
      background-color: #4CAF50;
      color: white;
    }
    
    .off-btn {
      background-color: #f44336;
      color: white;
    }
    
    .control-btn:hover {
      transform: translateY(-3px);
      box-shadow: 0 6px 12px rgba(0, 0, 0, 0.15);
    }
    
    .nav-links {
      margin-top: 30px;
      text-align: center;
    }
    
    .nav-links a {
      display: inline-block;
      color: #0066cc;
      text-decoration: none;
      margin: 0 10px;
    }
    
    .nav-links a:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <h1>SMART ALARM SYSTEM</h1>
  
  <hr>
  <hr>
  
  <h2>Manual Buzzer Control</h2>
  
  <div class="appliance-controls">
    <form action="/?LED_ON=YES" method="POST">
      <input type="submit" class="control-btn on-btn" name="SUBMIT" value="TURN ON BUZZER">
    </form>
    
    <form action="/?LED_OFF=YES" method="POST">
      <input type="submit" class="control-btn off-btn" name="SUBMIT" value="TURN OFF BUZZER">
    </form>
  </div>
  
  <div class="nav-links">
    <a href="/">HOME</a>
    <a href="/login?LOGOUT=YES">Log Out</a>
  </div>
</body>
</html>
)=====";
    
    const char ALARM_SET[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Alarm Set</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
          display: flex;
          flex-direction: column;
          align-items: center;
          min-height: 100vh;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin: 40px 0;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .success-message {
          background-color: #dff0d8;
          border: 1px solid #d6e9c6;
          color: #3c763d;
          padding: 20px;
          border-radius: 8px;
          margin: 30px 0;
          text-align: center;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          animation: fadeIn 1s;
        }
        
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
          padding: 10px 15px;
          border: 1px solid #0066cc;
          border-radius: 4px;
          transition: background-color 0.2s, color 0.2s;
        }
        
        .nav-links a:hover {
          background-color: #0066cc;
          color: white;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="success-message">
        <h2>Your alarm has been set for @@DAY_SET@@ day!</h2>
      </div>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
    const char ALARM_COPY[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Alarms Copied</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
          display: flex;
          flex-direction: column;
          align-items: center;
          min-height: 100vh;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin: 40px 0;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .success-message {
          background-color: #dff0d8;
          border: 1px solid #d6e9c6;
          color: #3c763d;
          padding: 20px;
          border-radius: 8px;
          margin: 30px 0;
          text-align: center;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          animation: fadeIn 1s;
        }
        
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
          padding: 10px 15px;
          border: 1px solid #0066cc;
          border-radius: 4px;
          transition: background-color 0.2s, color 0.2s;
        }
        
        .nav-links a:hover {
          background-color: #0066cc;
          color: white;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="success-message">
        <h2>Alarms have been successfully copied from @@SOURCE_DAY@@ to all other days!</h2>
      </div>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
    const char PRETIME_SET[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Time Set</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
          display: flex;
          flex-direction: column;
          align-items: center;
          min-height: 100vh;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin: 40px 0;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .success-message {
          background-color: #dff0d8;
          border: 1px solid #d6e9c6;
          color: #3c763d;
          padding: 20px;
          border-radius: 8px;
          margin: 30px 0;
          text-align: center;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          animation: fadeIn 1s;
        }
        
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
          padding: 10px 15px;
          border: 1px solid #0066cc;
          border-radius: 4px;
          transition: background-color 0.2s, color 0.2s;
        }
        
        .nav-links a:hover {
          background-color: #0066cc;
          color: white;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="success-message">
        <h2>Your Time and Date has been set!</h2>
      </div>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
    const char APPLIANCES_ON[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Appliances On</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
          display: flex;
          flex-direction: column;
          align-items: center;
          min-height: 100vh;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin: 40px 0;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .success-message {
          background-color: #dff0d8;
          border: 1px solid #d6e9c6;
          color: #3c763d;
          padding: 20px;
          border-radius: 8px;
          margin: 30px 0;
          text-align: center;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          animation: fadeIn 1s;
        }
        
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
          padding: 10px 15px;
          border: 1px solid #0066cc;
          border-radius: 4px;
          transition: background-color 0.2s, color 0.2s;
        }
        
        .nav-links a:hover {
          background-color: #0066cc;
          color: white;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="success-message">
        <h2>Buzzer is now ON!</h2>
      </div>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
    const char APPLIANCES_OFF[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Appliances Off</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 0 15px;
          display: flex;
          flex-direction: column;
          align-items: center;
          min-height: 100vh;
        }
        
        h1, h2, h3 {
          color: #0066cc;
          text-align: center;
        }
        
        h2 {
          text-decoration: underline;
          margin: 40px 0;
        }
        
        hr {
          border: 0;
          height: 2px;
          background-color: #3399ff;
          width: 90%;
          margin: 10px auto;
        }
        
        .success-message {
          background-color: #f2dede;
          border: 1px solid #ebccd1;
          color: #a94442;
          padding: 20px;
          border-radius: 8px;
          margin: 30px 0;
          text-align: center;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          animation: fadeIn 1s;
        }
        
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        
        .nav-links {
          margin-top: 30px;
          text-align: center;
        }
        
        .nav-links a {
          display: inline-block;
          color: #0066cc;
          text-decoration: none;
          margin: 0 10px;
          padding: 10px 15px;
          border: 1px solid #0066cc;
          border-radius: 4px;
          transition: background-color 0.2s, color 0.2s;
        }
        
        .nav-links a:hover {
          background-color: #0066cc;
          color: white;
        }
      </style>
    </head>
    <body>
      <h1>SMART ALARM SYSTEM</h1>
      
      <hr>
      <hr>
      
      <div class="success-message">
        <h2>Buzzer is now OFF!</h2>
      </div>
      
      <div class="nav-links">
        <a href="/">HOME</a>
        <a href="/login?LOGOUT=YES">Log Out</a>
      </div>
    </body>
    </html>
    )=====";
    
const char COPY_FORM[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Alarm System - Copy Alarms</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: powderblue;
          margin: 0;
          padding: 20px;
        }
        
        .container {
          max-width: 600px;
          margin: 0 auto;
          background-color: white;
          padding: 30px;
          border-radius: 8px;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        
        h2 {
          color: #0066cc;
          text-align: center;
          margin-bottom: 30px;
        }
        
        .form-group {
          margin-bottom: 25px;
        }
        
        .form-group label {
          display: block;
          margin-bottom: 10px;
          font-weight: bold;
          color: #333;
        }
        
        .form-control {
          width: 100%;
          padding: 12px;
          border: 1px solid #ddd;
          border-radius: 4px;
          font-size: 16px;
        }
        
        .alert {
          padding: 15px;
          margin-bottom: 20px;
          border: 1px solid transparent;
          border-radius: 4px;
        }
        
        .alert-warning {
          background-color: #fcf8e3;
          border-color: #faebcc;
          color: #8a6d3b;
        }
        
        .btn {
          display: inline-block;
          font-weight: 400;
          text-align: center;
          white-space: nowrap;
          vertical-align: middle;
          cursor: pointer;
          padding: 10px 16px;
          font-size: 14px;
          line-height: 1.42857143;
          border-radius: 4px;
          text-decoration: none;
        }
        
        .btn-primary {
          color: #fff;
          background-color: #337ab7;
          border: none;
        }
        
        .btn-primary:hover {
          background-color: #286090;
        }
        
        .btn-default {
          color: #333;
          background-color: #fff;
          border: 1px solid #ccc;
        }
        
        .btn-default:hover {
          background-color: #e6e6e6;
        }
        
        .button-group {
          display: flex;
          justify-content: space-between;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h2>Copy Alarms</h2>
        <form action="/" method="get">
          <div class="form-group">
            <label for="SOURCE_DAY">Copy alarms from:</label>
            <select class="form-control" id="SOURCE_DAY" name="SOURCE_DAY">
              <option value="monday" @@MON_SELECTED@@>Monday</option>
              <option value="tuesday" @@TUE_SELECTED@@>Tuesday</option>
              <option value="wednesday" @@WED_SELECTED@@>Wednesday</option>
              <option value="thursday" @@THU_SELECTED@@>Thursday</option>
              <option value="friday" @@FRI_SELECTED@@>Friday</option>
              <option value="saturday" @@SAT_SELECTED@@>Saturday</option>
              <option value="sunday" @@SUN_SELECTED@@>Sunday</option>
            </select>
          </div>
          <div class="alert alert-warning">
            <strong>Warning!</strong> This will overwrite all alarms on other days with the selected day's alarms.
          </div>
          <div class="button-group">
            <button type="submit" class="btn btn-primary" name="COPY_ALARMS" value="1">Copy</button>
            <a href="/" class="btn btn-default">Cancel</a>
          </div>
        </form>
      </div>
    </body>
    </html>
    )=====";