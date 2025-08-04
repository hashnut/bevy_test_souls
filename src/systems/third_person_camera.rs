use std::f32::consts::PI;
use bevy::{
    input::mouse::{MouseMotion, MouseWheel},
    prelude::*,
    window::{CursorGrabMode, PrimaryWindow},
};

/// Third person camera component with enhanced features
#[derive(Component)]
pub struct ThirdPersonCamera {
    /// Flag to indicate if the aim functionality is turned on.
    pub aim_enabled: bool,
    /// The mouse aim button binding.
    pub aim_button: MouseButton,
    /// The speed at which aiming occurs.
    pub aim_speed: f32,
    /// The zoom level of the aim zooming functionality.
    pub aim_zoom: f32,
    /// Flag to indicate if the cursor lock toggle functionality is turned on.
    pub cursor_lock_toggle_enabled: bool,
    /// Flag to indicate if the cursor is in a locked state or not.
    pub cursor_lock_active: bool,
    /// The cursor lock toggle key binding.
    pub cursor_lock_key: KeyCode,
    /// Mouse x/y sensitivity
    pub sensitivity: Vec2,
    /// Flag to indicate if the orbiting functionality is controlled when a mouse button is being pressed.
    pub mouse_orbit_button_enabled: bool,
    /// The mouse button binding to control when the orbiting occurs.
    pub mouse_orbit_button: MouseButton,
    /// Flag to indicate whether there is a camera offset applied or not.
    pub offset_enabled: bool,
    /// The camera offset relative to the camera target.
    pub offset: Offset,
    /// Inverts the x value of the offset.
    pub offset_toggle_enabled: bool,
    /// The key binding of the offset toggle
    pub offset_toggle_key: KeyCode,
    /// The speed at which the x offset will transition.
    pub offset_toggle_speed: f32,
    /// Flag to indicate whether a camera zoom is applied or not.
    pub zoom_enabled: bool,
    /// The min/max bounds of the camera.
    pub zoom: Zoom,
    /// The speed at which the scroll wheel zooms in or out.
    pub zoom_sensitivity: f32,
}

impl Default for ThirdPersonCamera {
    fn default() -> Self {
        ThirdPersonCamera {
            aim_enabled: false,
            aim_button: MouseButton::Right,
            aim_speed: 3.0,
            aim_zoom: 0.7,
            cursor_lock_key: KeyCode::Tab,  // 스페이스는 회피에 사용되므로 Tab으로 변경
            cursor_lock_toggle_enabled: true,
            cursor_lock_active: true,
            sensitivity: Vec2::new(1.0, 1.0),
            mouse_orbit_button_enabled: false,
            mouse_orbit_button: MouseButton::Middle,
            offset_enabled: false,
            offset: Offset::new(0.5, 0.4),
            offset_toggle_enabled: false,
            offset_toggle_speed: 7.5,  // 5.0 * 1.5 = 7.5
            offset_toggle_key: KeyCode::KeyE,
            zoom_enabled: true,
            zoom: Zoom::new(5.0, 15.0), // 소울라이크 게임에 맞게 조정
            zoom_sensitivity: 1.0,
        }
    }
}

/// Sets the zoom bounds (min & max)
pub struct Zoom {
    pub min: f32,
    pub max: f32,
    pub radius: f32,
    pub radius_copy: Option<f32>,
}

impl Zoom {
    pub fn new(min: f32, max: f32) -> Self {
        Self {
            min,
            max,
            radius: (min + max) / 2.0,
            radius_copy: None,
        }
    }
}

/// Offset the camera behind the player
pub struct Offset {
    pub offset: (f32, f32),
    pub offset_copy: (f32, f32),
    pub is_transitioning: bool,
}

impl Offset {
    pub fn new(x: f32, y: f32) -> Self {
        Self {
            offset: (x, y),
            offset_copy: (x, y),
            is_transitioning: false,
        }
    }
}

/// The desired target for the third person camera to look at
#[derive(Component)]
pub struct ThirdPersonCameraTarget;

/// Sync the camera with the player
pub fn sync_player_camera(
    player_q: Query<&Transform, With<ThirdPersonCameraTarget>>,
    mut cam_q: Query<(&mut ThirdPersonCamera, &mut Transform), Without<ThirdPersonCameraTarget>>,
) {
    let Ok(player) = player_q.get_single() else {
        return;
    };
    let Ok((cam, mut cam_transform)) = cam_q.get_single_mut() else {
        return;
    };

    // Calculate the desired camera translation based on radius and offset
    let rotation_matrix = Mat3::from_quat(cam_transform.rotation);

    // Apply the offset if enabled
    let mut offset = Vec3::ZERO;
    if cam.offset_enabled {
        offset = rotation_matrix.mul_vec3(Vec3::new(cam.offset.offset.0, cam.offset.offset.1, 0.0));
    }

    let desired_translation =
        rotation_matrix.mul_vec3(Vec3::new(0.0, 0.0, cam.zoom.radius)) + offset;

    // Update the camera translation
    cam_transform.translation = desired_translation + player.translation;
}

/// Handle mouse orbit camera movement
pub fn orbit_mouse(
    window_q: Query<&Window, With<PrimaryWindow>>,
    mut cam_q: Query<(&ThirdPersonCamera, &mut Transform), With<ThirdPersonCamera>>,
    mouse: Res<ButtonInput<MouseButton>>,
    mut mouse_evr: EventReader<MouseMotion>,
) {
    let Ok((cam, mut cam_transform)) = cam_q.get_single_mut() else {
        return;
    };

    // Only orbit if cursor is locked or mouse button is pressed
    if !cam.cursor_lock_active {
        return;
    }

    if cam.mouse_orbit_button_enabled && !mouse.pressed(cam.mouse_orbit_button) {
        return;
    }

    let mut rotation = Vec2::ZERO;
    for ev in mouse_evr.read() {
        rotation = ev.delta;
    }

    rotation *= cam.sensitivity;

    if rotation.length_squared() > 0.0 {
        let window = window_q.single();
        let delta_x = rotation.x / window.width() * PI * cam.sensitivity.x;
        let delta_y = rotation.y / window.height() * PI * cam.sensitivity.y;
        
        let yaw = Quat::from_rotation_y(-delta_x);
        let pitch = Quat::from_rotation_x(-delta_y);
        cam_transform.rotation = yaw * cam_transform.rotation; // rotate around global y axis

        // Calculate the new rotation without applying it to the camera yet
        let new_rotation = cam_transform.rotation * pitch;

        // Check if new rotation will cause camera to go beyond the vertical bounds
        let up_vector = new_rotation * Vec3::Y;
        if up_vector.y > 0.0 {
            cam_transform.rotation = new_rotation;
        }
    }

    let rot_matrix = Mat3::from_quat(cam_transform.rotation);
    cam_transform.translation = rot_matrix.mul_vec3(Vec3::new(0.0, 0.0, cam.zoom.radius));
}

/// Handle mouse wheel zoom
pub fn zoom_mouse(
    mut scroll_evr: EventReader<MouseWheel>, 
    mut cam_q: Query<&mut ThirdPersonCamera>
) {
    let mut scroll = 0.0;
    for ev in scroll_evr.read() {
        scroll += ev.y;
    }

    if let Ok(mut cam) = cam_q.get_single_mut() {
        if scroll.abs() > 0.0 {
            let new_radius =
                cam.zoom.radius - scroll * cam.zoom.radius * 0.1 * cam.zoom_sensitivity;
            cam.zoom.radius = new_radius.clamp(cam.zoom.min, cam.zoom.max);
        }
    }
}

/// Handle aim functionality
pub fn aim(
    mut cam_q: Query<
        (&mut ThirdPersonCamera, &Transform),
        (With<ThirdPersonCamera>, Without<ThirdPersonCameraTarget>),
    >,
    mouse: Res<ButtonInput<MouseButton>>,
    mut player_q: Query<&mut Transform, With<ThirdPersonCameraTarget>>,
    time: Res<Time>,
) {
    let Ok((mut cam, cam_transform)) = cam_q.get_single_mut() else {
        return;
    };

    if !cam.aim_enabled {
        return;
    }

    let Ok(mut player_transform) = player_q.get_single_mut() else {
        return;
    };

    // Check if aim button was pressed
    let is_aiming = mouse.pressed(cam.aim_button);

    if is_aiming {
        // Rotate player to face direction they are aiming
        player_transform.look_to(*cam_transform.forward(), Vec3::Y);

        let desired_zoom = cam.zoom.min * cam.aim_zoom;

        // Save original zoom for restoration
        if cam.zoom.radius_copy.is_none() {
            cam.zoom.radius_copy = Some(cam.zoom.radius);
        }

        let zoom_factor =
            (cam.zoom.radius_copy.unwrap() / cam.aim_zoom) * cam.aim_speed * time.delta_secs();

        // Stop zooming in if current radius is less than desired zoom
        if cam.zoom.radius <= desired_zoom || cam.zoom.radius - zoom_factor <= desired_zoom {
            cam.zoom.radius = desired_zoom;
        } else {
            cam.zoom.radius -= zoom_factor;
        }
    } else {
        if let Some(radius_copy) = cam.zoom.radius_copy {
            let zoom_factor = (radius_copy / cam.aim_zoom) * cam.aim_speed * time.delta_secs();

            // Stop zooming out if current radius is greater than original radius
            if cam.zoom.radius >= radius_copy || cam.zoom.radius + zoom_factor >= radius_copy {
                cam.zoom.radius = radius_copy;
                cam.zoom.radius_copy = None;
            } else {
                cam.zoom.radius += zoom_factor;
            }
        }
    }
}

/// Toggle camera offset (left/right shoulder view)
pub fn toggle_offset(
    mut cam_q: Query<&mut ThirdPersonCamera, With<ThirdPersonCamera>>,
    keys: Res<ButtonInput<KeyCode>>,
    time: Res<Time>,
) {
    let Ok(mut cam) = cam_q.get_single_mut() else {
        return;
    };

    if !cam.offset_toggle_enabled {
        return;
    }

    if keys.just_pressed(cam.offset_toggle_key) {
        // Switch direction by inverting the offset flag
        cam.offset.is_transitioning = !cam.offset.is_transitioning;
    }

    // Determine the transition speed based on direction
    let transition_speed = if cam.offset.is_transitioning {
        -cam.offset_toggle_speed
    } else {
        cam.offset_toggle_speed
    };

    // Update the offset based on the direction and time
    cam.offset.offset.0 = (cam.offset.offset.0 + transition_speed * time.delta_secs())
        .clamp(-cam.offset.offset_copy.0, cam.offset.offset_copy.0);
}

/// Toggle cursor lock
pub fn toggle_cursor(
    mut cam_q: Query<&mut ThirdPersonCamera>,
    keys: Res<ButtonInput<KeyCode>>,
    mut window_q: Query<&mut Window, With<PrimaryWindow>>,
) {
    let Ok(mut cam) = cam_q.get_single_mut() else {
        return;
    };

    if !cam.cursor_lock_toggle_enabled {
        return;
    }

    if keys.just_pressed(cam.cursor_lock_key) {
        cam.cursor_lock_active = !cam.cursor_lock_active;
    }

    if let Ok(mut window) = window_q.get_single_mut() {
        if cam.cursor_lock_active {
            window.cursor_options.grab_mode = CursorGrabMode::Locked;
            window.cursor_options.visible = false;
        } else {
            window.cursor_options.grab_mode = CursorGrabMode::None;
            window.cursor_options.visible = true;
        }
    }
}