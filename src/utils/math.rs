use bevy::prelude::*;

pub fn distance_3d(a: Vec3, b: Vec3) -> f32 {
    (a - b).length()
}

pub fn distance_2d(a: Vec3, b: Vec3) -> f32 {
    let a_2d = Vec2::new(a.x, a.z);
    let b_2d = Vec2::new(b.x, b.z);
    (a_2d - b_2d).length()
}

pub fn lerp_vec3(a: Vec3, b: Vec3, t: f32) -> Vec3 {
    a + (b - a) * t
}

pub fn normalize_angle(angle: f32) -> f32 {
    let mut normalized = angle % (2.0 * std::f32::consts::PI);
    if normalized < 0.0 {
        normalized += 2.0 * std::f32::consts::PI;
    }
    normalized
}

pub fn angle_between_vectors(a: Vec3, b: Vec3) -> f32 {
    let dot = a.dot(b);
    let mag_a = a.length();
    let mag_b = b.length();
    
    if mag_a == 0.0 || mag_b == 0.0 {
        return 0.0;
    }
    
    (dot / (mag_a * mag_b)).acos()
}