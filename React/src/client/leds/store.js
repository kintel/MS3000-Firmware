import * as actions from './actions';

import color from 'sc-color';

import {ledsCursor} from '../state';
import {register} from '../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.toggleLed:
      ledsCursor(cursor => {
        const {ledId, active} = data;
        return cursor.setIn(['list', ledId, 'active'], !active);
      });

      break;

    case actions.activateLed:
      ledsCursor(cursor => {
        const {ledId, e} = data;
        return cursor.setIn(['list', ledId, 'active'], true);
      });

      break;

    case actions.changeLed:
      ledsCursor(cursor => {
        const {id, value} = data;

        const leds = cursor
          .get('list')
          .map(val => {
            if (val && val.get('active')) {
              val = val.set('value', value);
            }
            return val;
          });

        return cursor.setIn(['list'], leds);
      });

      break;

    case actions.updateActiveLeds:

      let byteString = '';

      ledsCursor(cursor => {
        const leds = cursor
          .get('list')
          .map(val => {
            if (val && val.get('active')) {
              return val.set('value', data);
            }

            return val;
          });

          leds.forEach(val => {
            const rgbColor = color(val.get('value'));
            byteString += String.fromCharCode(rgbColor.red());
            byteString += String.fromCharCode(rgbColor.green());
            byteString += String.fromCharCode(rgbColor.blue());
            byteString += String.fromCharCode(0xff);
          });

          byteString = btoa(byteString);
          fetch(`http://magicshifter.local/leds?b=${byteString}`);

        return cursor.setIn(['list'], leds);
      });

      break;

    case actions.toggleAllLeds:
      ledsCursor(cursor => {
        const leds = cursor
          .get('list')
          .map((val, key) => val.set('active', !val.get('active')));

        return cursor.setIn(['list'], leds);
      });

      break;

    case actions.selectAllLeds:
      ledsCursor(cursor => {
        const leds = cursor
          .get('list')
          .map((val, key) => val.set('active', true));

        return cursor.setIn(['list'], leds);
      });

      break;

    case actions.deselectAllLeds:
      ledsCursor(cursor => {
        const leds = cursor
          .get('list')
          .map((val, key) => val.set('active', false));

        return cursor.setIn(['list'], leds);
      });

      break;
  
    case actions.changeActiveColor:
      ledsCursor(cursor => {
        return cursor.set('activeColor', data);
      });

      break;

  }
});
