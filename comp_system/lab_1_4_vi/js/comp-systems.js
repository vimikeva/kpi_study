$(document).ready(function() {
	$(".slider").toArray().forEach(function(item) {
		$(item).noUiSlider({
			start: parseInt($(item).attr("data-value")),
			connect: "lower",
			step: parseInt($(item).attr("data-step")),
			range: {
				'min': parseInt($(item).attr("data-min")),
				'max': parseInt($(item).attr("data-max"))
			}
		});

		displaySlider(item);
      First();
		$(item).on("slide", function() {
			displaySlider(this);
		});
	});

});

function displaySlider(item) {
	$("label[for=" + $(item).attr("id") + "]").text(parseInt($(item).val()) + $(item).attr("data-units"));
}

function getRandomInt(min, max) {
	return Math.floor(Math.random() * (max - min)) + min;
}

function Task(id) {
	if (processors.length > 0) {
		var weaker = processors[0];
		this.complexity = getRandomInt(weaker.speed * 10, weaker.speed * 200);

		this.canBePerformed = [];

		for (var i in processors) {
			if (Math.random() < 0.5) {
				this.canBePerformed.push(processors[i].num);
			}
		}

		if (this.canBePerformed.length == 0) {
			this.canBePerformed.push(getRandomInt(1, processors.length + 1));
		}

		this.id = id + JSON.stringify(this.canBePerformed);
	}
	this.performed = 0;
	this.complete = false;
}

Task.prototype.perform = function(speed) {
	this.performed =
		(this.performed + speed > this.complexity) ?
		this.complexity : this.performed + speed;

	this.complete = this.performed == this.complexity;
};

Task.prototype.toString = function() {
	return "Task #" + this.id + " (" + (this.performed * 100 / this.complexity).toFixed(2) + "%)";
};

function Processor(speed, num) {
	this.speed = speed;
	this.num = num;
	this.task = null;
	this.idle = true;
	this.totalTasks = 0;
	this.completedTask = 0;
}

Processor.prototype.toString = function() {
	return "Procesor #" + this.num + " (" + this.speed + " op/ms)";
};

Processor.prototype.setTask = function(task) {
	this.task = task;
	this.idle = false;
};

Processor.prototype.tick = function() {
	if (this.task != null) {
		this.task.perform(this.speed);
		if (this.task.complete) {
			this.task = null;
			this.idle = true;
			this.completedTask++;
		}
		return true;
	}
	return false;
};

function Computer(num, taskManager) {
	this.processors = $.map(processors, function(obj) {
		return $.extend(true, {}, obj);
	});

	this.num = num;
	this.total = 0;
	this.max = 0;
	this.performed = 0;
	this.effectivity = 100;
	this.taskList = [];
	this.taskManager = new taskManager(this);
}

Computer.prototype.dump = function() {
	var num = this.num;
	$("#comp-" + this.num + "-performed").text((this.performed / 1000000).toFixed(2) + " млн");
	$("#comp-" + this.num + "-total").text((this.total / 1000000).toFixed(2) + " млн");
	$("#comp-" + this.num + "-max").text((this.max / 1000000).toFixed(2) + " млн");
	$("#comp-" + this.num + "-effectivity").text((this.performed * 100 / this.total).toFixed(4) + "%");
	$("#comp-" + this.num + "-effectivity2").text((this.performed * 100 / this.max).toFixed(4) + "%");

	var log = "><b class='text-warning'>Computer #" + num +
		" (" + this.performed + "|" + this.total + "|" + this.max + ") " +
		(this.performed * 100 / this.total).toFixed(4) + "% " +
		(this.performed * 100 / this.max).toFixed(4) + "% | TIME: " +
		currentTime.toString() + "/" + endTime.toString() + "</b>\n";

	var planning = this.taskManager.planning;
	var plannerNum = this.taskManager.strongest || this.taskManager.weakest;

	this.processors.forEach(function(item) {

		log += "--><span class='text-primary'>" + item + "</span> " +
			(item.task == null ? "<span class='text-danger'>idle</span>" : "<span class='text-info'>" + item.task + "</span>") +
			(planning && item.num == plannerNum ? " <span class='text-success'>planning</span>\n" : "\n");

		if (item.task == null) {
			$("#comp-" + num + "-p-" + item.num).css({
				"width": 0
			});
		}
		else {
			$("#comp-" + num + "-p-" + item.num).css({
				"width": ((item.task.performed * 100 /
					item.task.complexity).toFixed(2) + "%")
			});
		}
	});

	if (num == 1) {
		$("#console").html("");
	}

	$("#console").append(log);
};

Computer.prototype.addTask = function(task) {
	if (task instanceof Task) {
		this.taskList.push(Object.create(task));
	}
};

Computer.prototype.tick = function() {

	this.taskManager.tick();

	var comp = this;

	this.processors.forEach(function(item) {
		if (item.tick()) {
			comp.performed += item.speed;
		}
		comp.total += item.speed;
		comp.max += item.speed;
	});
};

function FIFO(comp) {
	this.taskList = comp.taskList;
	this.processors = comp.processors;
}

FIFO.prototype.tick = function() {
	if (this.taskList.length > 0) {
		var task = this.taskList[0];

		for (var i = 0; i < this.processors.length; i++) {
			if (task.canBePerformed.indexOf(this.processors[i].num) != -1) {
				if (this.processors[i].idle) {
					this.processors[i].setTask(task);
					this.taskList.shift();
					break;
				}
			}
		}

	}
};

function Weakest(comp) {
	this.taskList = comp.taskList;
	this.processors = comp.processors;

	var weakest = 1;
	comp.processors.forEach(function(item) {
		if (item.speed < comp.processors[weakest - 1].speed) {
			weakest = item.num;
		}
	});

	this.weakest = weakest;

	comp.tick = function() {
		this.taskManager.tick();

		var comp = this;

		this.processors.forEach(function(item) {
			if (item.tick()) {
				comp.performed += item.speed;
			}
			if (item.num != comp.taskManager.weakest) {
				comp.total += item.speed;
			}
			comp.max += item.speed;
		});
	};
}

Weakest.prototype.tick = function() {
	if (this.taskList.length > 0) {

		outer: for (var i = 0; i < this.processors.length; i++) {

			var proc = this.processors[i];

			if (!proc.idle || proc.num == this.weakest) {
				continue;
			}

			for (var j = 0; j < this.taskList.length; j++) {
				var task = this.taskList[j];
				if (task.canBePerformed.indexOf(proc.num) != -1) {
					proc.setTask(task);
					this.taskList.splice(j, 1);
					break outer;
				}
			}
		}

	}
};

function Strongest(comp) {
	this.taskList = comp.taskList;
	this.processors = comp.processors;
	this.timer = 4;
	this.planning = true;

	var strongest = 1;
	comp.processors.forEach(function(item) {
		if (item.speed > comp.processors[strongest - 1].speed) {
			strongest = item.num;
		}
	});

	this.strongest = strongest;

	this.timerTick = function() {

		this.timer--;
		if (this.timer < 0) {
			this.planning = !this.planning;
			this.timer = this.planning ? 4 : 20;
		}
	};

	comp.tick = function() {

		this.taskManager.tick();

		var comp = this;

		this.processors.forEach(function(item) {

			comp.max += item.speed;

			if (!(item.num == comp.taskManager.strongest && comp.taskManager.planning)) {

				if (item.tick()) {
					comp.performed += item.speed;
				}
				comp.total += item.speed;
			}
		});

		this.taskManager.timerTick();
	};
}

Strongest.prototype.tick = function() {

	if (this.planning && this.taskList.length > 0) {

		outer: for (var i = 0; i < this.processors.length; i++) {

			var proc = this.processors[i];

			if (!proc.idle) {
				continue;
			}

			for (var j = 0; j < this.taskList.length; j++) {
				var task = this.taskList[j];
				if (task.canBePerformed.indexOf(proc.num) != -1) {
					proc.setTask(task);
					this.taskList.splice(j, 1);
					continue outer;
				}
			}
		}
	}
};

function StrongestAuto(comp) {
	this.taskList = comp.taskList;
	this.processors = comp.processors;
	this.planning = false;

	var strongest = 1;
	comp.processors.forEach(function(item) {
		if (item.speed > comp.processors[strongest - 1].speed) {
			strongest = item.num;
		}
	});

	this.strongest = strongest;

	comp.tick = function() {

		this.taskManager.tick();

		var comp = this;

		this.processors.forEach(function(item) {

			comp.max += item.speed;

			if (!(item.num == comp.taskManager.strongest && comp.taskManager.planning)) {

				if (item.tick()) {
					comp.performed += item.speed;
				}
				comp.total += item.speed;
			}
		});
	};
}

StrongestAuto.prototype.tick = function() {

	this.planning = false;

	if (this.taskList.length > 0) {

		outer: for (var i = 0; i < this.processors.length; i++) {

			var proc = this.processors[i];

			if (!proc.idle) {
				continue;
			}

			this.planning = true;

			for (var j = 0; j < this.taskList.length; j++) {
				var task = this.taskList[j];
				if (task.canBePerformed.indexOf(proc.num) != -1) {
					proc.setTask(task);
					this.taskList.splice(j, 1);
					continue outer;
				}
			}
		}
	}
};

var currentTime = 0;
var endTime = 100;
var pause = false;
var intervalId = null;

function Pause() {
	$("#pause")
		.toggleClass("paused")
		.toggleClass("btn-primary")
		.toggleClass("btn-info");
	pause = !pause;
}

var processors = [];
var comps = [];

function Start() {
    Second();
	processors = [];
	processors.push(new Processor(parseInt($("#slider-p1").val()), 1));
	processors.push(new Processor(parseInt($("#slider-p2").val()), 2));
	processors.push(new Processor(parseInt($("#slider-p3").val()), 3));
	processors.push(new Processor(parseInt($("#slider-p4").val()), 4));
	processors.push(new Processor(parseInt($("#slider-p5").val()), 5));

	comps = [];
	comps.push(new Computer(1, FIFO));
	comps.push(new Computer(2, Weakest));
	comps.push(new Computer(3, Strongest));
	comps.push(new Computer(4, StrongestAuto));

	endTime = parseInt($("#slider-time").val());

	clearInterval(intervalId);
	intervalId = setInterval(function() {
		if (!pause) {
			Step();
		}
	}, $("#slider-step").val());
}

function Step() {
	var newTask = false;

	if (parseFloat($("#slider-prob").val()) / 100 > Math.random()) {
		newTask = new Task(getRandomInt(10000, 50000));
	}

	comps.forEach(function(item) {
		item.tick();
		item.dump();
		if (newTask) {
			item.addTask(newTask);
		}
	});

	currentTime++;

	if (currentTime > endTime) {
		Stop();
	}
}

function Stop() {
    First();
	clearInterval(intervalId);
	comps = [];
	currentTime = 0;
}

function First() {
    document.getElementById("planning-task").style.display='';
    document.getElementById("planning-task-1").style.display='';
    document.getElementById("display-logging").style.display='none';
    document.getElementById("text-logging").style.display='none';
}

function Second() {
    document.getElementById("planning-task").style.display='none';
    document.getElementById("planning-task-1").style.display='none';
    document.getElementById("display-logging").style.display='';
    document.getElementById("text-logging").style.display='none';
}

function Third() {
    document.getElementById("planning-task").style.display='none';
    document.getElementById("planning-task-1").style.display='none';
    document.getElementById("display-logging").style.display='none';
    document.getElementById("text-logging").style.display='';
}
