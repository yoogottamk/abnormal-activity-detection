import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() => runApp(ESWRemote());

class ESWRemote extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'ESWRemote',
        theme: ThemeData(primarySwatch: Colors.blue),
        home: HomePage(title: 'Management Console'));
  }
}

class HomePage extends StatefulWidget {
  HomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  bool _serverUp = false, _buzzerEnabled = false, _espUP = false;
  int _buttonPress = 0;
  final _serverIP = '10.2.8.12', _serverPort = '9999';

  /*
     This enable/disable was handled differently
     to handle concurrency issues.

     A button like "Toggle current state" would have caused
     confusion if multiple people were using this app at the
     same time

     resp == '1' => the request was processed by the server
     So, the update rule for _disableBuzz looks a bit weird
   */
  void _enableBuzz() {
    _request('/enable/').then((resp) => setState(() {
          _buzzerEnabled = resp == '1';
        }));
  }

  void _disableBuzz() {
    _request('/disable/').then((resp) => setState(() {
          _buzzerEnabled = (resp == '1') ? false : true;
        }));
  }

  void _getStatus() {
    setState(() {
      _serverUp = false;
    });

    _request('/test/').then((resp) => setState(() {
          _serverUp = resp == '1';
        }));

    _request('/status/').then((resp) => setState(() {
          _buzzerEnabled = resp == '1';
        }));

    _request('/esp-status').then((resp) => setState(() {
          _espUP = resp == '1';
        }));
  }

  @override
  void initState() {
    super.initState();
    _getStatus();
  }

  Future<String> _request(String endpoint) async {
    final response =
        await http.get('http://' + _serverIP + ':' + _serverPort + endpoint);

    if (response.statusCode == 200)
      return response.body;
    else
      return '';
  }

  void _soundBuzzer() {
    _request('/BUZZ/');
  }

  // All widgets

  Widget _buzzStatus() {
    return Container(
      margin: EdgeInsets.only(bottom: 48.0),
      width: MediaQuery.of(context).size.width / 2,
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: <Widget>[
          Container(
              margin: EdgeInsets.only(top: 16.0),
              child: Text('ESP: ' + (_espUP ? 'ACTIVE' : 'DOWN'),
                  style: TextStyle(fontFamily: 'Inconsolata', fontSize: 24.0))),
          Container(
            padding: EdgeInsets.all(16.0),
            child: _espUP
                ? Icon(
                    Icons.check_circle,
                    color: Colors.green,
                    size: 48.0,
                  )
                : Icon(Icons.error, color: Colors.red, size: 48.0),
          ),
        ],
      ),
    );
  }

  Widget _buzzControlText() {
    return Container(
      padding: EdgeInsets.only(bottom: 24.0),
      child: Text(
        'BUZZER ' + (_buzzerEnabled ? 'ENABLED' : 'DISABLED'),
        style: TextStyle(fontFamily: 'Inconsolata', fontSize: 24.0),
      ),
    );
  }

  Widget _buzzControlButton() {
    return Container(
      margin: EdgeInsets.only(bottom: 48.0),
      child: RaisedButton(
        child: Text(_buzzerEnabled ? 'DISABLE' : 'ENABLE'),
        onPressed: _buzzerEnabled ? _disableBuzz : _enableBuzz,
      ),
    );
  }

  Widget _buzzControlIcon() {
    return Container(
        margin: EdgeInsets.only(bottom: 32.0),
        child: SizedBox(
          height: 96.0,
          width: 96.0,
          child: IconButton(
            icon: Icon(_buzzerEnabled ? Icons.alarm_on : Icons.alarm_off,
                size: 64.0),
            onPressed: () {
              setState(() {
                _buttonPress++;
              });
            },
          ),
        ));
  }

  Widget _buzzScam() {
    return Visibility(
      visible: _buttonPress > 10,
      child: RaisedButton(
        child: Text('BUZZ'),
        onPressed: _soundBuzzer,
      ),
    );
  }

  Widget _controls() {
    if (!_serverUp) {
      return Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Container(
            margin: EdgeInsets.only(bottom: 32.0),
            child: Icon(Icons.cloud_off, size: 48.0)
          ),
          Text('The server is\nnot reachable!',
              style: TextStyle(fontFamily: 'Inconsolata', fontSize: 24.0)),
        ],
      );
    } else {
      return Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          _buzzStatus(),
          _buzzControlText(),
          _buzzControlButton(),
          _buzzControlIcon(),
          _buzzScam(),
        ],
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
        child: _controls(),
      ),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.refresh),
        onPressed: _getStatus,
      ),
    );
  }
}
