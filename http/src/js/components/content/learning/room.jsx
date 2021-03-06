import React from "react";
import {learn, stopLearning} from "../../../api/learn";
import {httpAsync} from "../../../api/networking";

function deleteRoom(locationName, trackingHost) {
    if (!PRODUCTION)
        console.log("Deleted room " + locationName);
    if (PRODUCTION)
        httpAsync("http://" + trackingHost + "/location?group=family&location=" + locationName, function (e) {
            console.log(e);
        }, "DELETE");
}

export default class Room extends React.Component {
    render () {
        const store = this.context.store;
        const state = store.getState();
        const locationName = this.props.location;
        const scanning = state.learning == locationName;
        const location = state.locations[locationName];

        const handleClick = () => {
            if (store.getState().learning == locationName)
                stopLearning();
            else
                learn(locationName);
        };

        // TODO Multiply accuracy with a decimal value (0-1) that approaches one as the fingerprint count rises

        return (
            <tr>
                <td style={{width: '26%'}}>{this.props.location.capitalizeFirstLetter()}</td>
                <td style={{width: '12%'}}>{location.count}</td>
                <td style={{width: '50%'}}>
                    <progress className="progress" value={location.accuracy} max="100">{location.accuracy}%</progress>
                </td>
                <td style={{width: '6%'}}>
                    <a className={scanning ? "button is-loading" : "button"} onClick={handleClick} style={{pointerEvents: 'auto', width: '60px'}}>
                        {scanning ? (<i className="fa fa-times-circle abort-learning"/>) : "Learn"}
                    </a>
                </td>
                <td style={{width: '6%'}}>
                    <a className="button is-danger"
                       onClick={deleteRoom.bind(this, locationName, state.config.trackingHost)}>Delete</a>
                </td>
            </tr>
        );
    }
}

Room.contextTypes = {
    store: React.PropTypes.object
};