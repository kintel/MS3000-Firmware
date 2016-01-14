import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {pixelEditorType, settingsType, layoutType} from 'utils/propTypes';

import RGBAInput from 'components/inputs/RGBAInput';
import NumberInput from 'components/inputs/NumberInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './PixelEditorSidebar.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, settings, layout} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
    settings: settings.toJS(),
    layout: layout.toJS(),
  };
};

export class PixelEditorSidebar extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired,
    setColumns: PropTypes.func.isRequired,

    pixelEditor: pixelEditorType,
    settings: settingsType,
    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.state = {
      visible: false,
    };
  }

  onClickShowSidebar(e) {
    this.setState({visible: !this.state.visible});
  }

  render() {
    const {
      setColorValue, setColumns, // actions
      pixelEditor, settings, layout, // state objects
    } = this.props;

    const {pixels, color, rows, visibleColumns, totalColumns} = pixelEditor;
    const {host, protocol} = settings;

    const {sidebar} = layout;
    const {width} = sidebar;

    const {visible} = this.state;

    let style = {
      container: {
        width,
      },
      rgba: {},
      button: {
        display: layout.width > 500 ? 'none' : 'inherit',
      },
    };

    if (layout.width < 500) {
      style.container.position = 'absolute';
      style.container.left = -width;

      style.content = {
        left: visible ? width : 0,
        top: 25,
      };

      if (layout.width < 330) {
        style.rgba = {
          position: 'fixed',
          bottom: 0,
          left: '1em',
        };
      }
    }

    return (
      <aside
        className={classes['container']}
        style={style.container}
      >
        <button
          style={style.button}
          className={classes['button']}
          onClick={e => this.onClickShowSidebar(e)}
        >
          {(visible ? '<<<' : '>>>')}
        </button>

        <div
          className={classes['content']}
          style={style.content}
        >
          <div className={classes['picker']}>

            <h3>Controls</h3>
            <ul>
              <li
                key='rgba'
                style={style.rgba}
              >
                <h5>Colors:</h5>
                <RGBAInput
                  color={color}
                  setColorValue={setColorValue}
                />
              </li>

              <li key='load'>
                <h5>Load Image from disk</h5>
                <ImageInput />
              </li>

              <li key='upload'>
                <h5>Upload</h5>
                <FileUploadInput
                  pixels={pixels}
                  height={rows}
                  width={visibleColumns}
                  totalWidth={totalColumns}
                  url={[protocol, host].join('://')}
                  text='send to MS3000'
                />
              </li>
            </ul>
          </div>

          <div className={classes['settings']}>
            <h3>Settings</h3>
            <ul>
              <li>
                <NumberInput
                  label='Columns:'
                  name='columns'
                  max={totalColumns}
                  val={visibleColumns}
                  action={setColumns}
                />
              </li>
            </ul>
          </div>
        </div>
      </aside>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorSidebar);
