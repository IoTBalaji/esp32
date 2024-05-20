
function initBody() {
	document.querySelector('#container').innerHTML = `<h1>ESP32 Web Server</h1>
	<h3>Using Access Point(AP) Mode</h3>
	<div data-led="1">
		<p>LED1 Status: <span>OFF</span></p>
		<button class="button" onclick="setLED(this)">ON</button>
	</div>
	<div data-led="2">
		<p>LED2 Status: <span>OFF</span></p>
		<button class="button" onclick="setLED(this)">ON</button>
	</div>
	<br>
	<button class="button" onclick="refreshStatus(this)">Refresh</button>`;
}
initBody();

function setLED(btn) {
	const ledNumber = parseInt(btn.closest('[data-led]').dataset.led);
	const isOn = btn.classList.contains('status-on');
	btn.setAttribute('disabled', '');
	const url = `/led/${ledNumber}/${isOn ? 'off' : 'on'}`;
	fetch(url, {
		method: 'POST'
	})
		.then(response => {
			if (response.ok) {
				const status = {};
				status[`led${ledNumber}`] = !isOn;
				updateStatus(status);
				btn.removeAttribute('disabled');
			} else {
				alert('Problem Occurred!');
			}
		});
}
function refreshStatus(btn) {
	btn.setAttribute('disabled', '');
	fetch('/led', {
		method: 'GET'
	})
		.then(response => response.json())
		.then(function (json) {
			updateStatus(json);
			btn.removeAttribute('disabled');
		});
}

function updateStatus(json) {
	if (json) {
		[1, 2].forEach(num => {
			if (json.hasOwnProperty(`led${num}`)) {
				document.querySelector(`[data-led="${num}"] span`).textContent = json[`led${num}`] ? 'ON' : 'OFF';
				document.querySelector(`[data-led="${num}"] button`).textContent = json[`led${num}`] ? 'OFF' : 'ON';
				if (json[`led${num}`]) {
					document.querySelector(`[data-led="${num}"] button`).classList.add('status-on');
				} else {
					document.querySelector(`[data-led="${num}"] button`).classList.remove('status-on');
				}
			}
		});
	}
}

refreshStatus();